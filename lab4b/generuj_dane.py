import random
import string

def get_random_operand():
    return random.choice(string.ascii_lowercase)

def generate_infix_expression(depth=0):
    """Generates a valid infix expression recursively."""
    if depth > 2:
        if random.random() > 0.3:
            return get_random_operand()
        else:
            return f"~{get_random_operand()}"

    ops = ['+', '-', '*', '/', '%', '<', '>', '^']
    
    choice = random.random()
    if choice < 0.3:
        return f"~{get_random_operand()}" if random.random() > 0.7 else get_random_operand()
    elif choice < 0.6: 
        return f"({generate_infix_expression(depth + 1)})"
    else: 
        left = generate_infix_expression(depth + 1)
        op = random.choice(ops)
        right = generate_infix_expression(depth + 1)
        return f"{left}{op}{right}"

def generate_infix_line():
    """Generates an INF: assignment or expression."""
    if random.random() > 0.5:
        line = f"{get_random_operand()}={generate_infix_expression()}"
    else:
        line = generate_infix_expression()
    
    junk = [' ', '#', '$', '!', '@']
    noisy_line = "".join(c + (random.choice(junk) if random.random() > 0.85 else "") for c in line)
    return f"INF: {noisy_line}"

def generate_onp_line():
    """Generates a valid RPN (ONP) expression."""
    stack_size = 0
    tokens = []
    
    target_len = random.randint(3, 7)
    while len(tokens) < target_len or stack_size != 1:
        if stack_size < 2 or (random.random() > 0.4 and len(tokens) < target_len):
            tokens.append(get_random_operand())
            stack_size += 1
        else:
            op = random.choice(['+', '-', '*', '/', '%', '<', '>', '^'])
            tokens.append(op)
            stack_size -= 1
            
    if random.random() > 0.7:
        tokens.append('~')
        
    onp_str = "".join(tokens)
    junk = [' ', '_', '?']
    noisy_onp = "".join(c + (random.choice(junk) if random.random() > 0.9 else "") for c in onp_str)
    return f"ONP: {noisy_onp}"

def main():
    formal_data = [
        "INF: a)+(b",
        "ONP: ab+a~a-+",
        "INF: a+b+(~a-a)",
        "INF: x=~~a+b*c",
        "INF: t = ~ a < x < ~b",
        "INF: ~a-~~b<c+d&!p|!!q",
        "INF: a^b*c-d<xp|q+x",
        "INF: x=~a*b/c-d+e%~f",
        "ONP: xabcdefg++++++=",
        "ONP: ab+c+d+e+f+g+",
        "ONP: abc++def++g++",
        "ONP: abc++def++g+++",
        "INF: x=a=b=c",
        "ONP: xabc===",
        "INF: x=a^b^c",
        "INF: x=a=b=c^d^e",
        "ONP: xabcde^^===",
        "INF: x=(a=(b=c^(d^e)))"
    ]
    
    num_random_lines = 32 
    total_lines = len(formal_data) + num_random_lines
    
    with open("dane.txt", "w", encoding="utf-8") as f:
        # Write the total count of test cases
        f.write(f"{total_lines}\n")
        
        for line in formal_data:
            f.write(line + "\n")
            
        for _ in range(num_random_lines):
            if random.random() > 0.5:
                line = generate_infix_line()
            else:
                line = generate_onp_line()
            
            line = line[:256]
            if len(line) < 6:
                line = line.ljust(6, '.')
            
            f.write(line + "\n")

    print(f"Generated dane.txt with {total_lines} total lines ({len(formal_data)} formal + {num_random_lines} random).")

if __name__ == "__main__":
    main()
