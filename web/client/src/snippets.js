// Every snippet here has been executed against the real interpreter binary
// and produces the output shown in its blurb. Keep them in sync with the
// language: no modulo operator, no mixed string+number concatenation,
// booleans print as 1/0, prefix ++ is not supported (postfix is).

export const DEFAULT_PROGRAM = `// Welcome to Codelang!
// A small language whose interpreter I wrote from scratch in C++.
// Press Run (or Ctrl+Enter) to execute this program on the server.

let greeting = "hello, ";
let target = "world";
print greeting + target;

function square(x) {
  return x * x;
}

for (let i = 1; i <= 5; i = i + 1) {
  print square(i);
}
`

export const SNIPPETS = [
  {
    id: 'hello',
    title: 'Hello, strings',
    blurb: 'Variables and string concatenation',
    code: `let who = "Codelang";
print "hello from " + who + "!";
print "strings " + "concatenate " + "with +";
`,
  },
  {
    id: 'math',
    title: 'Numbers & math',
    blurb: 'Arithmetic, precedence, decimals',
    code: `let x = 10;
var y = x * 2 + 5;
print y;
print (x + y) * 2;
print 10 / 4;   // decimals work: 2.5
`,
  },
  {
    id: 'functions',
    title: 'Functions & recursion',
    blurb: 'fib(12) computed recursively',
    code: `function fib(n) {
  if (n < 2) { return n; }
  return fib(n - 1) + fib(n - 2);
}

print fib(12);

function max(a, b) {
  if (a > b) { return a; }
  return b;
}

print max(3, 9);
`,
  },
  {
    id: 'forloop',
    title: 'For loops',
    blurb: 'Classic counting loop',
    code: `// squares of 1..6
for (let i = 1; i <= 6; i = i + 1) {
  print i * i;
}
`,
  },
  {
    id: 'whileif',
    title: 'While + if/else',
    blurb: 'Countdown with a branch',
    code: `let n = 5;
while (n > 0) {
  if (n == 3) {
    print "three!";
  } else {
    print n;
  }
  n = n - 1;
}
print "liftoff";
`,
  },
  {
    id: 'increment',
    title: 'Increment / decrement',
    blurb: 'Postfix ++ and -- statements',
    code: `let i = 5;
i++;
i++;
print i;   // 7

let j = 3;
j--;
print j;   // 2
`,
  },
  {
    id: 'booleans',
    title: 'Comparisons & booleans',
    blurb: 'Booleans are numbers: 1 and 0',
    code: `print 2 > 1;      // 1 means true
print 2 == 3;     // 0 means false
print true;       // true is just 1
print !false;     // and ! negates
`,
  },
  {
    id: 'errors',
    title: 'Break it (error handling)',
    blurb: 'The interpreter catches bad programs',
    code: `// The interpreter refuses politely instead of crashing.
// Try each line by itself:

print 1 / 0;

// print undefinedVariable;
// let = ;
// print "unterminated string;
`,
  },
]
