import redatam
from zipfile import ZipFile

def test_redatam():
    zip = ZipFile('tests/galapagos.zip')
    zip.extractall('tests/galapagos')
    zip.close

    glp = redatam.read_redatam("tests/galapagos/cg15.dic")

    # check the list length
    assert len(glp) == 7

    # check the number of rows and columns
    assert glp["canton"].shape == (3, 4), f"Expected shape (3, 4), but got {glp['canton'].shape}"
    assert glp["canton_labels_nomcant"].shape == (1, 2), f"Expected shape (1, 2), but got {glp['canton_labels_nomcant'].shape}"

    glp2 = redatam.read_redatam("tests/galapagos/cg15.dicX")

    # check the list length
    assert len(glp2) == 7

    # check the number of rows and columns
    assert glp2["canton"].shape == (3, 4), f"Expected shape (3, 4), but got {glp2['canton'].shape}"
    assert glp2["canton_labels_nomcant"].shape == (3, 2), f"Expected shape (1, 2), but got {glp2['canton_labels_nomcant'].shape}"

if __name__ == "__main__":
    test_redatam()