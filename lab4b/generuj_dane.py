import random
import string

def get_random_operand():
    return random.choice(string.ascii_lowercase)

def generate_infix_expression(depth=0):
    """Generuje poprawne wyrażenie infiksowe rekurencyjnie."""
    if depth > 2:  # Ograniczenie złożoności
        if random.random() > 0.3:
            return get_random_operand()
        else:
            return f"~{get_random_operand()}"

    ops = ['+', '-', '*', '/', '%', '<', '>', '^']
    
    choice = random.random()
    if choice < 0.3: # Operand lub unarny
        return f"~{get_random_operand()}" if random.random() > 0.7 else get_random_operand()
    elif choice < 0.6: # Nawiasy
        return f"({generate_infix_expression(depth + 1)})"
    else: # Operacja binarna
        left = generate_infix_expression(depth + 1)
        op = random.choice(ops)
        right = generate_infix_expression(depth + 1)
        return f"{left}{op}{right}"

def generate_infix_line():
    """Generuje linię INF: przypisanie lub samo wyrażenie."""
    if random.random() > 0.5:
        # Instrukcja przypisania: operand = wyrażenie
        line = f"{get_random_operand()}={generate_infix_expression()}"
    else:
        line = generate_infix_expression()
    
    # Dodanie szumu (znaki do usunięcia wg treści zadania)
    junk = [' ', '#', '$', '!', '@']
    noisy_line = "".join(c + (random.choice(junk) if random.random() > 0.85 else "") for c in line)
    return f"INF: {noisy_line}"

def generate_onp_line():
    """Generuje poprawne wyrażenie ONP."""
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
        
    if random.random() > 0.5:
        tokens.insert(0, get_random_operand())
        tokens.append('=')
        
    onp_str = "".join(tokens)
    junk = [' ', '_', '?']
    noisy_onp = "".join(c + (random.choice(junk) if random.random() > 0.9 else "") for c in onp_str)
    return f"ONP: {noisy_onp}"

def main():
    num_lines = 50
    
    with open("dane.txt", "w", encoding="utf-8") as f:
        f.write(f"{num_lines}\n")
        for _ in range(num_lines):
            if random.random() > 0.5:
                line = generate_infix_line()
            else:
                line = generate_onp_line()
            
            # Przytnij do 256 i upewnij się że min 6
            line = line[:256]
            if len(line) < 6:
                line = line.ljust(6, '.')
            
            f.write(line + "\n")

    print(f"Plik dane.txt został wygenerowany ({num_lines} linii).")

if __name__ == "__main__":
    main()
