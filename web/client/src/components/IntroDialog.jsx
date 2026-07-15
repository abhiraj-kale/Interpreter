import {
  Box, Button, Chip, Dialog, DialogActions, DialogContent, IconButton, Link, Stack, Typography,
} from '@mui/material'
import PlayArrowRoundedIcon from '@mui/icons-material/PlayArrowRounded'
import AutoStoriesRoundedIcon from '@mui/icons-material/AutoStoriesRounded'
import CodeRoundedIcon from '@mui/icons-material/CodeRounded'
import AccountTreeRoundedIcon from '@mui/icons-material/AccountTreeRounded'
import VerifiedRoundedIcon from '@mui/icons-material/VerifiedRounded'
import CloseIcon from '@mui/icons-material/Close'
import { GOLD } from '../theme'

function Line({ icon, children }) {
  return (
    <Stack direction="row" spacing={1.25} alignItems="flex-start">
      <Box sx={{ color: GOLD, mt: 0.2, display: 'flex' }}>{icon}</Box>
      <Typography variant="body2" color="text.secondary">{children}</Typography>
    </Stack>
  )
}

export default function IntroDialog({ open, onClose, profileUrl, repoUrl }) {
  return (
    <Dialog open={open} onClose={onClose} maxWidth="sm" fullWidth
      PaperProps={{ sx: { backgroundImage: 'none', border: '1px solid', borderColor: 'divider' } }}>
      <IconButton onClick={onClose} aria-label="close"
        sx={{ position: 'absolute', top: 10, right: 10, color: 'text.secondary' }}>
        <CloseIcon />
      </IconButton>
      <DialogContent sx={{ pt: 4 }}>
        <Stack direction="row" spacing={1.5} alignItems="center" sx={{ mb: 1.5, pr: 4 }}>
          <AutoStoriesRoundedIcon sx={{ fontSize: 32, color: GOLD }} />
          <Box>
            <Typography variant="h6" sx={{ fontFamily: '"Fraunces", Georgia, serif' }}>
              Codelang Playground
            </Typography>
            <Typography variant="body2" color="text.secondary">
              A language interpreter built from scratch in C++ by{' '}
              <Link href={profileUrl} target="_blank" rel="noreferrer" sx={{ color: GOLD }}>Abhiraj Kale</Link>
            </Typography>
          </Box>
        </Stack>

        <Typography variant="body2" color="text.secondary" sx={{ mb: 2 }}>
          This is a real programming language, Codelang, with its own lexer, parser, and
          interpreter, all written in C++17 with no parser-generator or third-party language
          tooling. Every time you press Run here, your code runs on the server inside that exact
          interpreter binary, not a simulation.
        </Typography>

        <Stack spacing={1.25}>
          <Line icon={<AccountTreeRoundedIcon fontSize="small" />}>
            Phase-based architecture: lexical scanning, recursive-descent parsing with operator
            precedence, AST construction, then a tree-walk interpreter.
          </Line>
          <Line icon={<CodeRoundedIcon fontSize="small" />}>
            Variables, functions with recursion, if/else, while/for loops, and string and
            numeric literals, a genuinely Turing-complete scripting environment.
          </Line>
          <Line icon={<VerifiedRoundedIcon fontSize="small" />}>
            Backed by Google Test with over 95% coverage on the scanner, parser, and
            interpreter, structured error handling, and smart pointers throughout.
          </Line>
        </Stack>
      </DialogContent>
      <DialogActions sx={{ px: 3, pb: 3, gap: 1 }}>
        <Chip label="C++17 · zero dependencies" size="small" variant="outlined" sx={{ mr: 'auto', color: 'text.secondary' }} />
        <Button component="a" href={repoUrl} target="_blank" rel="noreferrer" color="inherit">
          View source
        </Button>
        <Button onClick={onClose} variant="contained" startIcon={<PlayArrowRoundedIcon />}>
          Close and Start Coding
        </Button>
      </DialogActions>
    </Dialog>
  )
}
