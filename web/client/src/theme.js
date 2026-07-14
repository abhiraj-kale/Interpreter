import { createTheme } from '@mui/material'

// Classy dark: obsidian surfaces, champagne-gold accent, serif logotype.
export const GOLD = '#d9b060'
export const GOLD_DIM = '#a8873f'

const theme = createTheme({
  palette: {
    mode: 'dark',
    background: { default: '#0b0d10', paper: '#12151a' },
    primary: { main: GOLD, contrastText: '#14110a' },
    success: { main: '#7fb88a' },
    error: { main: '#e5734f' },
    divider: 'rgba(160,150,130,0.14)',
    text: { primary: '#e8e6e1', secondary: '#9b9890' },
  },
  shape: { borderRadius: 10 },
  typography: {
    fontFamily: '"Inter", system-ui, -apple-system, sans-serif',
    h6: { fontWeight: 700 },
    subtitle2: { fontWeight: 600 },
  },
  components: {
    MuiPaper: {
      styleOverrides: {
        root: {
          backgroundImage: 'none',
          border: '1px solid rgba(160,150,130,0.14)',
        },
      },
    },
    MuiButton: {
      styleOverrides: { root: { textTransform: 'none', fontWeight: 600 } },
    },
    MuiChip: {
      styleOverrides: { root: { fontWeight: 500 } },
    },
  },
})

export default theme
