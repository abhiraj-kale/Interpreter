// Codelang playground server.
// POST /api/run pipes the submitted program into the compiled C++ interpreter
// (script mode: `codelang --script` reads the whole program on stdin) and
// returns whatever it printed. Untrusted code is contained by the language
// itself (no file/network/system access exists in Codelang) plus a hard
// timeout and an output cap here.
import express from 'express'
import { spawn } from 'child_process'
import { existsSync } from 'fs'
import path from 'path'
import { fileURLToPath } from 'url'

const __dirname = path.dirname(fileURLToPath(import.meta.url))

const PORT = process.env.PORT || 8001
const BIN = process.env.CODELANG_BIN || path.join(__dirname, 'bin', 'codelang')
const RUN_TIMEOUT_MS = Number(process.env.RUN_TIMEOUT_MS || 5000)
const MAX_CODE_BYTES = 64 * 1024
const MAX_OUTPUT_BYTES = 64 * 1024

const app = express()
app.use(express.json({ limit: MAX_CODE_BYTES }))

app.get('/api/health', (_req, res) => {
  res.json({ ok: true, interpreter: existsSync(BIN) })
})

app.post('/api/run', (req, res) => {
  const code = req.body?.code
  if (typeof code !== 'string' || code.trim().length === 0) {
    return res.status(400).json({ error: 'code (non-empty string) is required' })
  }
  if (!existsSync(BIN)) {
    return res.status(500).json({ error: 'interpreter binary missing on server' })
  }

  const started = process.hrtime.bigint()
  const child = spawn(BIN, ['--script'], {
    stdio: ['pipe', 'pipe', 'pipe'],
    timeout: RUN_TIMEOUT_MS,
    killSignal: 'SIGKILL',
  })

  let out = ''
  let truncated = false
  const collect = (chunk) => {
    if (out.length >= MAX_OUTPUT_BYTES) {
      if (!truncated) {
        truncated = true
        child.kill('SIGKILL') // runaway print loop: stop it, keep what we have
      }
      return
    }
    out += chunk.toString('utf8')
  }
  child.stdout.on('data', collect)
  child.stderr.on('data', collect)

  child.on('error', (err) => {
    res.status(500).json({ error: `failed to start interpreter: ${err.message}` })
  })

  child.on('close', (exitCode, signal) => {
    if (res.headersSent) return
    const durationMs = Number(process.hrtime.bigint() - started) / 1e6
    const timedOut = signal === 'SIGKILL' && !truncated
    res.json({
      output: out.slice(0, MAX_OUTPUT_BYTES),
      exitCode,
      timedOut,
      truncated,
      durationMs: Math.round(durationMs * 10) / 10,
    })
  })

  child.stdin.on('error', () => {}) // process may die before we finish writing
  child.stdin.write(code)
  child.stdin.end()
})

// serve the built playground UI (client/dist locally, ./static in Docker)
const staticDir = [
  process.env.STATIC_DIR,
  path.join(__dirname, 'static'),
  path.join(__dirname, '..', 'client', 'dist'),
].filter(Boolean).find((p) => existsSync(p))

if (staticDir) {
  app.use(express.static(staticDir))
  app.get(/^\/(?!api\/).*/, (_req, res) => res.sendFile(path.join(staticDir, 'index.html')))
}

app.listen(PORT, () => {
  console.log(`codelang playground listening on :${PORT} (interpreter: ${BIN})`)
})
