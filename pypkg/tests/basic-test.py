import redatam
from zipfile import ZipFile

def test_redatam():
    zip = ZipFile('tests/galapagos.zip')
    zip.extractall('tests/galapagos')
    zip.close

    glp = redatam.read_redatam("tests/galapagos/cg15.dic")
    print(glp.keys())
    print(glp["provin"])

    glp2 = redatam.read_redatam("tests/galapagos/cg15.dicX")
    print(glp2.keys())
    print(glp2["provin"])

if __name__ == "__main__":
    test_redatam()