try:
    from ._redatam import export_redatam_to_dict
except ImportError:
    raise ImportError("The compiled extension '_redatam' could not be imported.")

from .read_redatam import read_redatam

__all__ = ["export_redatam_to_dict", "read_redatam"]
