import pandas as pd
import numpy as np
from chardet import detect

def detect_encoding(strings):
    valid_strings = " ".join([s for s in strings if s and not pd.isna(s)])
    if valid_strings:
        detected_encoding = detect(valid_strings.encode())
        if detected_encoding:
            return detected_encoding['encoding']
    return "UTF-8"

def dict_to_dataframe(x):
    if isinstance(x, dict):
        for key in x:
            if isinstance(x[key], dict):
                x[key] = pd.DataFrame.from_dict(x[key], orient='columns')
    return x

def fix_encoding_recursive(x, encoding):
    if isinstance(x, dict):
        return {k: fix_encoding_recursive(v, encoding) for k, v in x.items()}
    elif isinstance(x, str):
        y = x.encode(encoding).decode('utf-8', 'ignore')
        y = y.replace("\u00c3\u00a1", "\u00e1")  # a with acute accent
        y = y.replace("\u00c3\u00a9", "\u00e9")  # e with acute accent
        y = y.replace("\u00c3\u00b3", "\u00f3")  # o with acute accent
        y = y.replace("\u00c3\u00ba", "\u00fa")  # u with acute accent
        y = y.replace("\u00c3\u00b1", "\u00f1")  # n with tilde
        y = y.replace("\u00b1", "\u00f1")       # n with tilde
        y = y.replace("\u00c3", "\u00ed")       # i with acute accent
        return y
    else:
        return x

def fix_encoding(x):
    strings = {str(i): item for i, sublist in enumerate(x.values()) for item in sublist if isinstance(item, str)}
    encoding = detect_encoding(strings.values())
    return {key: fix_encoding_recursive(value, encoding) for key, value in x.items()}

def trim_and_clean_internal(x):
    if isinstance(x, str):
        x = x.strip()
        x = " ".join(x.split())
        return np.nan if x == "" else x
    return x

def trim_and_clean(x):
    for key in x:
        if isinstance(x[key], pd.DataFrame):
            char_cols = x[key].select_dtypes(include=['object']).columns
            for col in char_cols:
                x[key][col] = x[key][col].apply(trim_and_clean_internal)
    return x

def harmonize_types(x):
    description_elements = [key for key in x if "_labels_" in key]
    for element in description_elements:
        entity = element.split("_labels_")[0]
        common_col = set(x[entity].columns).intersection(set(x[element].columns))
        for col in common_col:
            target_class = x[entity][col].dtype
            if target_class != x[element][col].dtype:
                x[element][col] = x[element][col].astype(target_class)
    return x
