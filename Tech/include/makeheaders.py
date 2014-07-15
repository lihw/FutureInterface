# ---------------------------------------------------------#
#   make headers from source files
#---------------------------------------------------------#

import sys
import os

def printUsage():
  print("Usage: makehaders.py [path-to-module] [include-path]")
  print("e.g., python makehaders.py ../src/user .")
  

if len(sys.argv) < 3:
  printUsage()
  sys.exit()

include_path = sys.argv[2]

# Check if the module path exists
module_path = os.path.abspath(sys.argv[1])
if not os.path.exists(module_path):
  print(sys.argv[1] + " doesn't exist.")
  sys.exit()

# Create the include path
module_name = os.path.split(module_path)[1]
module_name = "P" + module_name.title()
module_include_path = os.path.join(include_path, module_name)
module_include_path = os.path.abspath(module_include_path)

print("Updating headers in " + module_include_path)

# Create the include path if it does not exists
if not os.path.exists(module_include_path):
  os.makedirs(module_include_path)
else:
  # Clean the old headers in the module folder.
  for afile in os.listdir(module_include_path):
    file_path = os.path.join(module_include_path, afile)
    try:
      if os.path.isfile(file_path) and afile.split('.')[-1] != 'swp':
        git_unlink_cmd = 'git rm -fq ' + file_path
        os.system(git_unlink_cmd);
    except:
      print(file_path + " can't be removed")
      sys.exit()

# Change to include path.
if not os.path.exists(module_include_path):
  os.makedirs(module_include_path)

os.chdir(module_include_path)


# We have two folder ready, module_path and module_include_path. Now we
# traverse recursively in module_path. For each .h file along the way,
# we create a header in the module_include_path which has the same prefix
# as the .h file and has only one line about including the .h path. For 
# each folder in the module_path, we create a module header in the 
# module_include_path which includes all headers in that module.

# Create a header
def make_header(header_file):
  (basename, filename) = os.path.split(header_file)
  # if it is not private header
  if filename.find('private') == -1:
    try:
      (prefix, suffix) = filename.split('.')
    except:
      print(filename + "is unsplitable")
      sys.exit()

    if suffix == "h":
      # only create header for .h file
      new_header_file = os.path.join(module_include_path, prefix + ".h")
      # find the include header in unix format.
      include_header = os.path.relpath(header_file).replace("\\", "/") 
      # construct the include string.
      include_string = "#include \"" + include_header + "\""
      # construct the header file and insert the include string into it.
      f = open(new_header_file, 'w')
      f.write(include_string)
      f.close()
      os.system("git add " + new_header_file) 
      # print(include_string)

# Create a module header
def make_module_header(module_path):
  (basename, dirname) = os.path.split(module_path)
  header_filename = "p" + dirname + "_module.h"
  new_module_header_file = os.path.join(module_include_path, header_filename)
  #print(new_module_header_file)
  print("Creating module header for " + dirname)
  f = open(new_module_header_file, 'w')
  # to avoid header to included more than once.
  include_macro = "P" + dirname.upper() + "_MODULE" + "_H"
  f.write("#ifndef " + include_macro + "\n")
  f.write("#define " + include_macro + "\n")
  files = [os.path.join(dp, fn) for dp, dn, filenames in os.walk(module_path) for fn in filenames]
  for afile in files:
    words = afile.split('.')
    if len(words) == 2 and words[1] == "h" and words[0].find('private') == -1:
      include_string = "#include \"" + os.path.relpath(afile).replace("\\", "/") + "\""
      f.write(include_string + "\n")
  f.write("#endif\n")
  f.close()
  os.system("git add " + new_module_header_file) 
  return header_filename


# Traverse the module folder and create headers for each file in the module
def make_headers(apath, depth):
  modules = []
  for item in os.listdir(apath):
    # skip trash directory
    if item.lower() == "trash" or item.lower() == "android" or item.lower() == "events" :
      continue
    item_path = os.path.join(apath, item)
    if os.path.isdir(item_path):
      make_headers(item_path, depth + 1)
      if depth == 1:
        modules.append(make_module_header(item_path))
    if os.path.isfile(item_path):
      make_header(item_path)
  return modules

modules = make_headers(module_path, 1)

# Add the layer header
layer_header_file = module_name.lower() + "_layer.h"
f = open(layer_header_file, 'w')
f.write("#ifndef " + module_name.upper() + "_LAYER_H\n")
f.write("#define " + module_name.upper() + "_LAYER_H\n")
for afile in modules:
  include_string = "#include \"" + afile + "\""
  f.write(include_string + "\n")
f.write("#endif\n")
f.close();
os.system("git add " + layer_header_file) 

# For each just created header, create its alias
# E.g., p_some_header.h -> PSomeHeader
#print("Creating alias for headers in module include path")
#headers = os.listdir(module_include_path)
#for item in headers:
#  (basename, filename) = os.path.split(item)
#  (prefix, suffix) = item.split('.')
#  words = prefix.split('_')
#  alias = ""
#  for w in words:
#    alias = alias + w.title()
#  alias = os.path.join(os.getcwd(), alias)
#  f = open(alias, 'w')
#  include_string = "#include \"" + filename + "\""
#  f.write(include_string + "\n")
#  f.close()




