import { Box, Chip, Paper, Stack, Typography } from '@mui/material'
import TerminalIcon from '@mui/icons-material/Terminal'

export default function OutputPanel({ result, running }) {
  const lines = (result?.output ?? '').split('\n')

  return (
    <Paper sx={{ display: 'flex', flexDirection: 'column', minHeight: 180, maxHeight: 260 }}>
      <Stack direction="row" alignItems="center" spacing={1}
        sx={{ px: 2, py: 0.75, borderBottom: '1px solid', borderColor: 'divider' }}>
        <TerminalIcon sx={{ fontSize: 16, color: 'text.secondary' }} />
        <Typography variant="subtitle2" sx={{ flex: 1, color: 'text.secondary' }}>Output</Typography>
        {result?.timedOut && <Chip size="small" color="error" variant="outlined" label="timed out (5s limit)" />}
        {result?.truncated && <Chip size="small" color="warning" variant="outlined" label="output truncated" />}
        {result && !result.timedOut && (
          <Chip size="small" variant="outlined" label={`${result.durationMs} ms`}
            sx={{ color: 'text.secondary', fontFamily: '"JetBrains Mono", monospace' }} />
        )}
      </Stack>
      <Box sx={{
        flex: 1, overflowY: 'auto', px: 2, py: 1.25,
        fontFamily: '"JetBrains Mono", monospace', fontSize: 13.5, lineHeight: 1.7,
        whiteSpace: 'pre-wrap', wordBreak: 'break-word',
      }}>
        {running && <Box component="span" sx={{ color: 'text.secondary' }}>running...</Box>}
        {!running && !result && (
          <Box component="span" sx={{ color: 'text.secondary', fontStyle: 'italic' }}>
            Run a program to see its output here.
          </Box>
        )}
        {!running && result && lines.map((line, i) => (
          <Box key={i} component="div"
            sx={{ color: line.startsWith('Error:') ? 'error.main' : '#cfe3cf' }}>
            {line || ' '}
          </Box>
        ))}
      </Box>
    </Paper>
  )
}
