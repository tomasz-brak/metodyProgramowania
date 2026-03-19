import random

def generate_test_case(
    filename: str = "in_custom.txt", 
    num_datasets: int = 1, 
    num_elements: int = 1000, 
    num_queries: int = 100
) -> None:
    MIN_VAL: int = -(2**48)
    MAX_VAL: int = 2**48
    
    with open(filename, 'w') as f:
        _ = f.write(f"{num_datasets}\n")
        
        for _ in range(num_datasets):
            _ = f.write(f"{num_elements}\n")
            
            elements: list[int] = []
            while len(elements) < num_elements:
                val: int = random.randint(MIN_VAL, MAX_VAL)
                repeats: int = random.randint(1, 10)
                elements.extend([val] * repeats)
                
            elements = elements[:num_elements]
            elements.sort()
            
            _ = f.write(" ".join(map(str, elements)) + "\n")
            
            _ = f.write(f"{num_queries}\n")
            
            queries: list[int] = []
            for _ in range(num_queries):
                if random.random() < 0.75:
                    queries.append(random.choice(elements))
                else:
                    queries.append(random.randint(MIN_VAL, MAX_VAL))
                    
            _ = f.write(" ".join(map(str, queries)) + "\n")


if __name__ == "__main__":
    file_name: str = "in_custom.txt"
    generate_test_case(
        filename=file_name,
        num_datasets=1,
        num_elements=1000,
        num_queries=100
    )
    print(f"Pomyślnie wygenerowano plik testowy: {file_name}")
