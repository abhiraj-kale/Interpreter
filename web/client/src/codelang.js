// Monaco registration for Codelang: tokenizer + editor theme.
// Keywords must stay in sync with the interpreter's scanner (src/scanner.cpp).

export const LANG_ID = 'codelang'

export function registerCodelang(monaco) {
  if (monaco.languages.getLanguages().some((l) => l.id === LANG_ID)) return

  monaco.languages.register({ id: LANG_ID })

  monaco.languages.setLanguageConfiguration(LANG_ID, {
    comments: { lineComment: '//' },
    brackets: [['{', '}'], ['(', ')']],
    autoClosingPairs: [
      { open: '{', close: '}' },
      { open: '(', close: ')' },
      { open: '"', close: '"' },
    ],
  })

  monaco.languages.setMonarchTokensProvider(LANG_ID, {
    keywords: [
      'let', 'var', 'print', 'function', 'return',
      'if', 'else', 'while', 'for', 'true', 'false', 'and', 'or',
    ],
    tokenizer: {
      root: [
        [/\/\/.*$/, 'comment'],
        [/"([^"\\]|\\.)*$/, 'string.invalid'],
        [/"/, 'string', '@string'],
        [/\d+(\.\d+)?/, 'number'],
        [/[a-zA-Z_]\w*/, { cases: { '@keywords': 'keyword', '@default': 'identifier' } }],
        [/(\+\+|--|==|!=|<=|>=|[+\-*/=<>!])/, 'operator'],
        [/[;,(){}]/, 'delimiter'],
      ],
      string: [
        [/[^"]+/, 'string'],
        [/"/, 'string', '@pop'],
      ],
    },
  })

  monaco.editor.defineTheme('codelang-dark', {
    base: 'vs-dark',
    inherit: true,
    rules: [
      { token: 'keyword', foreground: 'd9b060', fontStyle: 'bold' },
      { token: 'string', foreground: '7fb88a' },
      { token: 'string.invalid', foreground: 'e5734f' },
      { token: 'number', foreground: 'c78f5f' },
      { token: 'comment', foreground: '5f6672', fontStyle: 'italic' },
      { token: 'identifier', foreground: 'd7dce4' },
      { token: 'operator', foreground: 'b3a37f' },
      { token: 'delimiter', foreground: '8b909a' },
    ],
    colors: {
      'editor.background': '#0e1116',
      'editor.lineHighlightBackground': '#161a21',
      'editorLineNumber.foreground': '#3d434d',
      'editorLineNumber.activeForeground': '#d9b060',
      'editorCursor.foreground': '#d9b060',
      'editor.selectionBackground': '#2a3040',
      'editorIndentGuide.background': '#1c212a',
    },
  })
}
