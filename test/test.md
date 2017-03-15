# Description of test.mini

## Pseudocode of test.mini
```c
void main(void) {
  char buf[8192];
  int i;

  for(i = 0; i < 10; i++); // dummy

  puts("9 - 6 = ?");
  gets(buf);

  if (strlen(buf) != 1) {
    puts("Bad input");
  } else {
    if (buf[0] < '0' || buf[0] > '9') {
      puts("Bad input");
    } else {
      if (buf[0] == 9-6+'0') { // buf[0] == '3'
        puts("Good!");
      } else {
        puts("Wrong!");
      }
    }
  }

  exit(0);
}
```

## Brief description
`test.mini` is a simple quiz program. Our VM has 14 instructions, and I used
12 instructions to test my VM implementation. Since `sub` and `gt` instructions
didn't need for implementing pseudocode at "Listing 1", I tried to use them
without any unnecessary logic(dummy logic). However, I still didn't cover yet
`move` and `ge` instruction without dummy logic. I covered them with
simple loop.

### SAMPLE INPUT 1
```
asdf
```

### SAMPLE OUTPUT 1
```
9 - 6 = ?
Bad input
```

### SAMPLE INPUT 2
```
12
```

### SAMPLE OUTPUT 2
```
9 - 6 = ?
Bad input
```

### SAMPLE INPUT 3
```
a
```

### SAMPLE OUTPUT 3
```
9 - 6 = ?
Bad input
```

### SAMPLE INPUT 4
```
1
```

### SAMPLE OUTPUT 4
```
9 - 6 = ?
Wrong!
```

### SAMPLE INPUT 5
```
1
```

### SAMPLE OUTPUT 5
```
9 - 6 = ?
Good!
```


