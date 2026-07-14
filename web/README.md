# Codelang Playground

A browser IDE for **Codelang**, the language this repo's C++ interpreter runs.
Type a program (or load one of the example snippets), hit Run, and the code
executes on the server inside the real compiled interpreter; whatever it
prints comes back to the output panel with the run time.

**Stack:** Node + Express backend that pipes code into the C++ binary
(script mode, 5s timeout, output cap) · React + Vite + Material UI frontend
with a Monaco editor speaking Codelang (custom syntax highlighting).

## How execution works

1. The interpreter gained a script mode (`codelang --script`): it reads a whole
   program from stdin, parses it with `Parser::parse()`, and runs it quietly
   (no REPL banner or prompts).
2. `POST /api/run { code }` spawns one interpreter process per request, writes
   the program to its stdin, and returns `{ output, exitCode, durationMs,
   timedOut, truncated }`.
3. Untrusted code is contained by the language itself (Codelang has no file,
   network, or system access), plus a 5-second kill timer and a 64KB output cap
   for runaway loops.

## Run locally

```bash
# 1. build the interpreter (from the repo root)
g++ -std=c++17 -O2 -Iinclude main.cpp src/*.cpp -o web/server/bin/codelang

# 2. backend
cd web/server && npm install && node server.js     # :8001

# 3. frontend (dev, hot reload)
cd web/client && npm install && npm run dev        # :5174, proxies /api
# or build it and let the backend serve it:
npm run build
```

> WSL tip: run the Node server from the Linux filesystem (not /mnt/c),
> otherwise module resolution is painfully slow.

## Docker / Render

```bash
# from the repo root
docker build -f web/Dockerfile -t codelang-playground .
docker run -p 8001:8001 codelang-playground
```

The repo root has a `render.yaml` blueprint: on
[dashboard.render.com](https://dashboard.render.com), New + -> Blueprint ->
connect this repo, and it deploys the whole thing on the free tier.
