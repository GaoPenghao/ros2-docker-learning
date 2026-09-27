from typing import List
from typing import Text

import launch
import yaml


class HasNodeParams(launch.Substitution):
    """
    Substitution that checks if a param file contains parameters for a node.

    Used in launch system
    """

    def __init__(
        self, source_file: launch.SomeSubstitutionsType, node_name: Text
    ) -> None:
        super().__init__()
        """
    Construct the substitution

    :param: source_file the parameter YAML file
    :param: node_name the name of the node to check
    """

        from launch.utilities import (
            normalize_to_list_of_substitutions,
        )  # import here to avoid loop

        self.__source_file = normalize_to_list_of_substitutions(source_file)
        self.__node_name = node_name

    @property
    def name(self) -> List[launch.Substitution]:
        """Getter for name."""
        return self.__source_file

    def describe(self) -> Text:
        """Return a description of this substitution as a string."""
        return ""

    def perform(self, context: launch.LaunchContext) -> Text:
        yaml_filename = launch.utilities.perform_substitutions(context, self.name)
        data = yaml.safe_load(open(yaml_filename, "r"))

        if self.__node_name in data.keys():
            return "True"
        return "False"
