import redatam

def test_redatam():

    glp = redatam.read_redatam("tests/uru2011mini/uru2011mini.dic")

    # check the list length
    assert len(glp) == 3

    # check the number of rows and columns
    assert glp["sexo"].shape == (38, 4), f"Expected shape (38, 4), but got {glp['sexo'].shape}"

    glp2 = redatam.read_redatam("tests/uru2011mini/uru2011mini.dicx")

    # check the list length
    assert len(glp2) == 2

    # check the number of rows and columns
    assert glp["sexo"].shape == (38, 4), f"Expected shape (38, 4), but got {glp['sexo'].shape}"

if __name__ == "__main__":
    test_redatam()