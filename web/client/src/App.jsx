import { useCallback, useRef, useState } from 'react'
import {
  AppBar, Box, Button, Chip, CircularProgress, Container, IconButton, Link,
  List, ListItemButton, ListItemText, Paper, Stack, Toolbar, Tooltip, Typography,
} from '@mui/material'
import Editor from '@monaco-editor/react'
import PlayArrowRoundedIcon from '@mui/icons-material/PlayArrowRounded'
import GitHubIcon from '@mui/icons-material/GitHub'
import MenuBookRoundedIcon from '@mui/icons-material/MenuBookRounded'
import RestartAltRoundedIcon from '@mui/icons-material/RestartAltRounded'
import AutoStoriesRoundedIcon from '@mui/icons-material/AutoStoriesRounded'
import { registerCodelang, LANG_ID } from './codelang'
import { DEFAULT_PROGRAM, SNIPPETS } from './snippets'
import OutputPanel from './components/OutputPanel'
import ReferenceDialog from './components/ReferenceDialog'
import IntroDialog from './components/IntroDialog'
import { GOLD } from './theme'

const REPO_URL = 'https://github.com/abhiraj-kale/Interpreter'
const PROFILE_URL = 'https://github.com/abhiraj-kale'

export default function App() {
  const [code, setCode] = useState(DEFAULT_PROGRAM)
  const [result, setResult] = useState(null)
  const [running, setRunning] = useState(false)
  const [refOpen, setRefOpen] = useState(false)
  const [introOpen, setIntroOpen] = useState(true)
  const [activeSnippet, setActiveSnippet] = useState(null)
  const editorRef = useRef(null)
  const codeRef = useRef(code)
  codeRef.current = code

  const run = useCallback(async () => {
    const source = codeRef.current
    if (!source.trim()) return
    setRunning(true)
    setResult(null)
    try {
      const res = await fetch('/api/run', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ code: source }),
      })
      const data = await res.json()
      if (!res.ok) throw new Error(data.error || `server error ${res.status}`)
      setResult(data)
    } catch (e) {
      setResult({ output: `Error: ${e.message}`, durationMs: 0 })
    } finally {
      setRunning(false)
    }
  }, [])

  const onEditorMount = (editor, monaco) => {
    editorRef.current = editor
    editor.addCommand(monaco.KeyMod.CtrlCmd | monaco.KeyCode.Enter, run)
    editor.focus()
  }

  const loadSnippet = (s) => {
    setCode(s.code)
    setActiveSnippet(s.id)
    setResult(null)
    editorRef.current?.focus()
  }

  return (
    <Box sx={{ minHeight: '100vh', display: 'flex', flexDirection: 'column' }}>
      <AppBar position="sticky" elevation={0}
        sx={{ bgcolor: 'rgba(11,13,16,0.9)', backdropFilter: 'blur(8px)', borderBottom: '1px solid', borderColor: 'divider' }}>
        <Toolbar sx={{ gap: 1.5 }}>
          <AutoStoriesRoundedIcon sx={{ color: GOLD }} />
          <Typography sx={{ fontFamily: '"Fraunces", Georgia, serif', fontWeight: 700, fontSize: '1.35rem', letterSpacing: 0.3 }}>
            Codelang
          </Typography>
          <Chip label="a language interpreted by C++ I wrote from scratch" size="small" variant="outlined"
            sx={{ color: 'text.secondary', display: { xs: 'none', md: 'flex' } }} />
          <Box sx={{ flex: 1 }} />
          <Button color="inherit" startIcon={<MenuBookRoundedIcon />} onClick={() => setRefOpen(true)}>
            Language reference
          </Button>
          <Tooltip title="Interpreter source on GitHub">
            <IconButton component="a" href={REPO_URL} target="_blank" rel="noreferrer" color="inherit">
              <GitHubIcon />
            </IconButton>
          </Tooltip>
        </Toolbar>
      </AppBar>

      <Container maxWidth="xl" sx={{ py: 2.5, flex: 1, display: 'flex', flexDirection: 'column' }}>
        <Box sx={{
          display: 'grid', gap: 2, flex: 1,
          gridTemplateColumns: { xs: '1fr', md: '250px 1fr' },
        }}>
          {/* snippet sidebar */}
          <Paper sx={{ alignSelf: 'start' }}>
            <Typography variant="subtitle2" sx={{ px: 2, pt: 1.5, pb: 0.5, color: GOLD }}>
              Example programs
            </Typography>
            <Typography variant="caption" sx={{ px: 2, color: 'text.secondary' }}>
              click one to load it
            </Typography>
            <List dense sx={{ py: 0.5 }}>
              {SNIPPETS.map((s) => (
                <ListItemButton key={s.id} selected={activeSnippet === s.id} onClick={() => loadSnippet(s)}
                  sx={{ '&.Mui-selected': { bgcolor: 'rgba(217,176,96,0.08)' } }}>
                  <ListItemText
                    primary={s.title}
                    secondary={s.blurb}
                    primaryTypographyProps={{ fontSize: 14, fontWeight: 600 }}
                    secondaryTypographyProps={{ fontSize: 12 }}
                  />
                </ListItemButton>
              ))}
            </List>
          </Paper>

          {/* editor + output */}
          <Stack spacing={2} sx={{ minWidth: 0 }}>
            <Paper sx={{ overflow: 'hidden' }}>
              <Stack direction="row" alignItems="center" spacing={1}
                sx={{ px: 2, py: 0.75, borderBottom: '1px solid', borderColor: 'divider' }}>
                <Typography variant="subtitle2" sx={{ color: 'text.secondary', flex: 1 }}>
                  main.clang
                </Typography>
                <Tooltip title="Reset to the welcome program">
                  <IconButton size="small" onClick={() => { setCode(DEFAULT_PROGRAM); setActiveSnippet(null); setResult(null) }}>
                    <RestartAltRoundedIcon fontSize="small" />
                  </IconButton>
                </Tooltip>
                <Button variant="contained" size="small" onClick={run} disabled={running}
                  startIcon={running ? <CircularProgress size={14} color="inherit" /> : <PlayArrowRoundedIcon />}>
                  {running ? 'Running' : 'Run'}
                </Button>
                <Typography variant="caption" sx={{ color: 'text.secondary', display: { xs: 'none', sm: 'block' } }}>
                  Ctrl+Enter
                </Typography>
              </Stack>
              <Editor
                height="52vh"
                language={LANG_ID}
                theme="codelang-dark"
                value={code}
                onChange={(v) => setCode(v ?? '')}
                beforeMount={registerCodelang}
                onMount={onEditorMount}
                options={{
                  fontFamily: '"JetBrains Mono", monospace',
                  fontSize: 14,
                  minimap: { enabled: false },
                  scrollBeyondLastLine: false,
                  padding: { top: 14 },
                  renderLineHighlight: 'all',
                  automaticLayout: true,
                  tabSize: 2,
                }}
              />
            </Paper>

            <OutputPanel result={result} running={running} />
          </Stack>
        </Box>

        <Typography variant="body2" sx={{ textAlign: 'center', color: 'text.secondary', mt: 2.5 }}>
          Every run executes on the server inside a C++17 tree-walk interpreter
          (lexer, recursive-descent parser, AST) built from scratch by{' '}
          <Link href={PROFILE_URL} target="_blank" rel="noreferrer" sx={{ color: GOLD }}>Abhiraj Kale</Link>
          {' '}with zero parser generators or dependencies ·{' '}
          <Link href={REPO_URL} target="_blank" rel="noreferrer" sx={{ color: GOLD }}>source & tests</Link>
        </Typography>
      </Container>

      <ReferenceDialog open={refOpen} onClose={() => setRefOpen(false)} />
      <IntroDialog open={introOpen} onClose={() => setIntroOpen(false)}
        profileUrl={PROFILE_URL} repoUrl={REPO_URL} />
    </Box>
  )
}
