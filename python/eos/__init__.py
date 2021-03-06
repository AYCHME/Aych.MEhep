# Copyright (c) 2018 Frederik Beaujean
# Copyright (c) 2017, 2018 Danny van Dyk
#
# This file is part of the EOS project. EOS is free software;
# you can redistribute it and/or modify it under the terms of the GNU General
# Public License version 2, as published by the Free Software Foundation.
#
# EOS is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 59 Temple
# Place, Suite 330, Boston, MA  02111-1307  USA

from _eos import *
from .data import *
from .plot import *
from .analysis import Analysis, BestFitPoint
from .constraint import Constraints
from .observable import Observables
from .parameter import Parameters
from .reference import References

import logging
logger = logging.getLogger()
logger.setLevel(logging.INFO)

__ipython__ = False
try:
    if __IPYTHON__:
        __ipython__ = True
        ip = get_ipython()
        html_formatter = ip.display_formatter.formatters['text/html']

        from .ipython import __format_Parameter, __format_KinematicVariable, __format_Observable, __format_ObservableEntry, __format_GoodnessOfFit
        html_formatter.for_type(Parameter, __format_Parameter)
        html_formatter.for_type(KinematicVariable, __format_KinematicVariable)
        html_formatter.for_type(Observable, __format_Observable)
        html_formatter.for_type(ObservableEntry, __format_ObservableEntry)
        html_formatter.for_type(GoodnessOfFit, __format_GoodnessOfFit)
except NameError as e:
    pass
