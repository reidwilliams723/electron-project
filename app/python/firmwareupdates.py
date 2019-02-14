####### Code for pulling code from git repo, checks for changes ########
# import git
# repo = git.Repo('/Users/reidwilliams/Repositories/ioteqKernel')
# repo.remotes.origin.pull()

# current = repo.head.commit
# repo.remotes.origin.pull()
# if current != repo.head.commit:
#     print("It changed")
# else:
#     print("No Change")

import subprocess
import os