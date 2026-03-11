from matplotlib import pyplot as plt

def plotEngine(engineType: str):
    with open(f"./out/Debug/{engineType}.txt", mode='r', encoding='utf-8') as file:
        linesPerfect: list[str] = file.readlines();

    with open(f"./data/silnik{engineType.upper()}/nm{engineType.upper()}.txt", mode='r', encoding='utf-8') as file:
        linesGot: list[str] = file.readlines();

    perfectPosY: list[float] = []
    perfectPosX: list[float] = []
    gotPosY: list[float] = []
    gotPosX: list[float] = []

    for number in linesPerfect[7].split('\t')[6:]:
        perfectPosY.append(float(number))
    
    for number in linesPerfect[9].split('\t')[6:]:
        perfectPosX.append(float(number))

    for number in linesGot[0].split('\t')[6:]:
        gotPosY.append(float(number))

    for number in linesGot[1].split('\t')[6:]:
        gotPosX.append(float(number))

    plt.plot(perfectPosX, perfectPosY, label='Idealne', linestyle='--', color='blue')
    plt.plot(gotPosX, gotPosY, label='Otrzymano', color='red')

    plt.xlabel('Pozycja X')
    plt.ylabel('Pozycja Y')
    plt.title(f'Wykres silnika: {engineType}')
    plt.legend()

    plt.savefig(f"{engineType}.png")
    plt.close() 

plotEngine("seak")
plotEngine("seb")
        

