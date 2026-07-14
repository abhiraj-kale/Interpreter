import {
  Dialog, DialogContent, DialogTitle, IconButton, Table, TableBody,
  TableCell, TableHead, TableRow, Typography, Box,
} from '@mui/material'
import CloseIcon from '@mui/icons-material/Close'

const mono = { fontFamily: '"JetBrains Mono", monospace', fontSize: 13 }

const ROWS = [
  ['Variables', 'let x = 10;  var y = 20;', 'let and var behave the same'],
  ['Print', 'print x + y;', 'the only output statement'],
  ['Strings', 'print "hi " + "there";', 'concatenation with + (strings only)'],
  ['Numbers', 'print 10 / 4;', 'all numbers are decimals: 2.5'],
  ['Functions', 'function add(a, b) { return a + b; }', 'return always takes a value'],
  ['Recursion', 'fib(n - 1) + fib(n - 2)', 'fully supported'],
  ['If / else', 'if (x > 5) { ... } else { ... }', 'parentheses required'],
  ['While', 'while (n > 0) { n = n - 1; }', ''],
  ['For', 'for (let i = 0; i < 5; i = i + 1) { ... }', ''],
  ['Increment', 'i++;  j--;', 'postfix only'],
  ['Booleans', 'print 2 > 1;', 'true prints as 1, false as 0'],
  ['Comments', '// anything after slashes', ''],
]

export default function ReferenceDialog({ open, onClose }) {
  return (
    <Dialog open={open} onClose={onClose} maxWidth="md" fullWidth>
      <DialogTitle sx={{ pr: 6 }}>
        Codelang in one page
        <Typography variant="body2" color="text.secondary">
          Statements end with a semicolon. No modulo, no arrays (yet). Mixing strings and
          numbers with + is a type error, on purpose.
        </Typography>
        <IconButton onClick={onClose} sx={{ position: 'absolute', top: 12, right: 12 }}>
          <CloseIcon />
        </IconButton>
      </DialogTitle>
      <DialogContent>
        <Table size="small">
          <TableHead>
            <TableRow>
              <TableCell sx={{ width: 120 }}>Feature</TableCell>
              <TableCell>Syntax</TableCell>
              <TableCell>Notes</TableCell>
            </TableRow>
          </TableHead>
          <TableBody>
            {ROWS.map(([feature, syntax, note]) => (
              <TableRow key={feature}>
                <TableCell sx={{ color: 'text.secondary' }}>{feature}</TableCell>
                <TableCell><Box component="code" sx={mono}>{syntax}</Box></TableCell>
                <TableCell sx={{ color: 'text.secondary' }}>{note}</TableCell>
              </TableRow>
            ))}
          </TableBody>
        </Table>
      </DialogContent>
    </Dialog>
  )
}
