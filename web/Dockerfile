# Codelang playground - single-container full-stack build.
# Build from the REPO ROOT (the interpreter sources live there):
#   docker build -f web/Dockerfile -t codelang-playground .
#   docker run -p 8001:8001 codelang-playground

# ---- stage 1: playground UI ----
FROM node:20-slim AS client
WORKDIR /client
COPY web/client/package.json web/client/package-lock.json* ./
RUN npm install --no-fund --no-audit
COPY web/client .
RUN npm run build

# ---- stage 2: the C++ interpreter ----
FROM debian:bookworm-slim AS interpreter
RUN apt-get update && apt-get install -y --no-install-recommends g++ libc6-dev && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY main.cpp CMakeLists.txt ./
COPY include ./include
COPY src ./src
RUN g++ -std=c++17 -O2 -Iinclude main.cpp src/scanner.cpp src/parser.cpp src/interpreter.cpp src/expr.cpp -o codelang

# ---- stage 3: runtime ----
FROM node:20-slim
WORKDIR /app
COPY web/server/package.json web/server/package-lock.json* ./
RUN npm install --omit=dev --no-fund --no-audit
COPY web/server/server.js .
COPY --from=interpreter /src/codelang ./bin/codelang
COPY --from=client /client/dist ./static
ENV STATIC_DIR=/app/static \
    CODELANG_BIN=/app/bin/codelang
EXPOSE 8001
CMD ["sh", "-c", "PORT=${PORT:-8001} node server.js"]
