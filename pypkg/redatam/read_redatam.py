# redatam/read_redatam.py

from os.path import abspath
from pandas import DataFrame
from . import export_redatam_to_dict
from .utils import (
    dict_to_dataframe,
    fix_encoding,
    trim_and_clean,
    harmonize_types
)

def read_redatam(dictionary):
    dictionary = abspath(dictionary)

    if dictionary.lower().endswith(('.dic', '.dicx')):
        res = export_redatam_to_dict(dictionary)
    else:
        raise ValueError("The file extension must be either .dic or .dicx")

    print("Fixing encoding...")

    # 1. convert dictionaries to dataframes
    res = dict_to_dataframe(res)
    res = {k: v for k, v in res.items() if not v.empty}

    # 2. fix encoding (e.g., ""C\xf3digo Region" -> "Código Region")
    res = fix_encoding(res)

    print("Tidying variables...")

    # 3. remove leading and trailing whitespaces and multiple whitespaces
    res = trim_and_clean(res)

    # 4. harmonize column types
    res = harmonize_types(res)

    # 5. tidy entries

    # put dict names in lowercase
    res = {k.lower(): v for k, v in res.items()}

    # put colnames in lowercase
    res = {k: v.rename(columns=str.lower) for k, v in res.items()}

    # sort by dict name
    res = dict(sorted(res.items()))

    return res
