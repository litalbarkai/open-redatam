import redatam

def test_redatam():
    glp = redatam.read_redatam("galapagos/cg15.dic")
    print(glp.keys())
    glp2 = redatam.read_redatam("galapagos/cg15.dicX")
    print(glp2.keys())

if __name__ == "__main__":
    test_redatam()