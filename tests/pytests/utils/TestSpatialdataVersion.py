#!/usr/bin/env python
#
# ======================================================================
#
# Brad T. Aagaard, U.S. Geological Survey
#
# This code was developed as part of the Computational Infrastructure
# for Geodynamics (http://geodynamics.org).
#
# Copyright (c) 2010-2016 University of California, Davis
#
# See COPYING for license information.
#
# ======================================================================
#

import unittest

from spatialdata.utils.utils import SpatialdataVersion

class TestSpatialdataVersion(unittest.TestCase):

  def test_isRelease(self):
    isRelease = SpatialdataVersion.isRelease()
    return


  def test_version(self):
    version = SpatialdataVersion.version()
    # Check that version is of the form X.X.X
    import re
    match = re.search("[0-9]+\.[0-9]+\.[0-9]+", version)
    self.failIf(match is None)
    return


  def test_gitVersion(self):
    revision = SpatialdataVersion.gitRevision()
    if SpatialdataVersion.isRelease():
      self.assertEqual("unknown", revision)
    else:
      # Check that revision is of the form v2.1.3-16-g9323114
      import re
      match = re.search("v[0-9]+\.[0-9]+\.[0-9]+", revision)
      if match is None:
        match = re.search("v[0-9]+\.[0-9]+\.[0-9]+-[0-9]+-g[0-9,a-z]+", revision)
      self.failIf(match is None)
    return


  def test_gitHash(self):
    tag = SpatialdataVersion.gitHash()
    if SpatialdataVersion.isRelease():
      self.assertEqual("unknown", tag)
    else:
      # Check form of hash
      import re
      match = re.search("[0-9,a-z]+", tag)
      self.failIf(match is None)
    return


  def test_gitDate(self):
    value = SpatialdataVersion.gitDate()
    if SpatialdataVersion.isRelease():
      self.assertEqual("unknown", value)
    else:
      # Check form of datetime
      import datetime
      fields = value.split()
      d = datetime.datetime.strptime(fields[0], "%Y-%m-%d")
      t = datetime.datetime.strptime(fields[1], "%H:%M:%S")
    return


  def test_gitBranch(self):
    branch = SpatialdataVersion.gitBranch()
    if SpatialdataVersion.isRelease():
      self.assertEqual("unknown", branch)
    else:
      self.failIf(len(branch) == 0)
    return


  def test_projVersion(self):
    version = SpatialdataVersion.projVersion()
    # Check that version is of the form XXX
    import re
    match = re.search("[0-9]+", version)
    self.failIf(match is None)    
    return




# End of file 
