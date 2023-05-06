##
## EPITECH PROJECT, 2022
## Epitech-Template
## File description:
## setup.py
##

GREEN_BOLD_ON = "\033[1;32m"
RED_BOLD_ON = "\033[1;31m"
COLOR_OFF = "\033[0m"

import sys, os, datetime

def isRepoUrlValid(url):
    command = f'git ls-remote "{url}" HEAD > /dev/null 2>&1'
    return os.system(command) == 0

def getUrl():
    url = None
    for arg in sys.argv:
        if "--url=" in arg:
            url = arg.split("=")[1]
    while True:
        if not isRepoUrlValid(url):
            if url is not None:
                print(f"Invalid remote url ({url}).")
            url = None
        else:
            break
        url = input("Remote url: ")
    return url

def getBinary():
    binary = None
    for arg in sys.argv:
        if "--binary=" in arg:
            binary = arg.split("=")[1]
    while True:
        if not binary or not binary.isalnum() or len(binary) == 0:
            if binary is not None:
                print(f"Invalid binary name ({binary}).")
            binary = None
        else:
            break
        binary = input("Binary name: ")
    return binary

def getTemplate():
    template = None
    for arg in sys.argv:
        if "--template=" in arg:
            template = arg.split("=")[1]
    while True:
        if not template or (template != "C" and template != "C++"):
            print(f"Invalid template name ({template}).")
            template = None
        else:
            break
        template = input("Template (C/C++): ")
    return template

def getVariables(url, binary, template):
    return {
        "__YEAR__":                str(datetime.datetime.now().year),
        "__BINARY_NAME__":         binary,
        "__FILE_TYPE__":           ".c" if template == "C" else ".cpp",
        "__COMPILER_TYPE__":       "CC" if template == "C" else "CXX",
        "__COMPILER__":            "gcc" if template == "C" else "g++",
        "__COMPILER_TYPE_FLAGS__": "CFLAGS" if template == "C" else "CXXFLAGS",
        "__COMPILER_FLAGS__":      "-Wall -Wextra -I ./include" + (" -std=c++20" if template == "C++" else ""),
        "__REMOTE_URL__":          url,
        "__CHECKER_SCRIPT__":      "./.github/" + ("c_checker.py" if template == "C" else "cpp_checker.py")
    }

def setupMakefile(variables):
    try:
        with open("Makefile", "r") as makefile:
            makefile_content = makefile.read()
            for variable in variables:
                makefile_content = makefile_content.replace(variable, variables[variable])
            with open("Makefile", "w") as makefile:
                makefile.write(makefile_content)
        with open("tests/Makefile", "r") as makefile:
            makefile_content = makefile.read()
            for variable in variables:
                makefile_content = makefile_content.replace(variable, variables[variable])
            with open("tests/Makefile", "w") as makefile:
                makefile.write(makefile_content)
    except:
        print(RED_BOLD_ON + "Cannot setup Makefile.")
        print(COLOR_OFF)
        exit(84)
    print(GREEN_BOLD_ON + "Makefile setup successfully." + COLOR_OFF)

def setupWorkflows(variables):
    try:
        for workflow in ["workflow.yml"]:
            filename = ".github/workflows/" + workflow
            with open(filename, "r") as workflow_file:
                workflow_content = workflow_file.read()
                for variable in variables:
                    workflow_content = workflow_content.replace(variable, variables[variable])
                with open(filename, "w") as workflow_file:
                    workflow_file.write(workflow_content)
    except:
        print(RED_BOLD_ON + "Cannot setup workflows.")
        print(COLOR_OFF)
        exit(84)
    print(GREEN_BOLD_ON + "Workflows setup successfully." + COLOR_OFF)

def setupGitignore(variables):
    try:
        with open(".gitignore", "r") as gitignore:
            gitignore_content = gitignore.read()
            for variable in variables:
                gitignore_content = gitignore_content.replace(variable, variables[variable])
            with open(".gitignore", "w") as gitignore:
                gitignore.write(gitignore_content)
    except:
        print(RED_BOLD_ON + "Cannot setup .gitignore.")
        print(COLOR_OFF)
        exit(84)
    print(GREEN_BOLD_ON + ".gitignore setup successfully." + COLOR_OFF)

def setupHooks(variables):
    # Setup commit-msg hook (copy ./.github/commit_msg.sh to ./.git/hooks/commit-msg and chmod +x)
    if os.system("cp ./.github/commit_msg.sh ./.git/hooks/commit-msg") != 0 or os.system("chmod +x ./.git/hooks/commit-msg") != 0:
        print(RED_BOLD_ON + "Cannot setup commit-msg hook.")
        print(COLOR_OFF)
        exit(84)
    print(GREEN_BOLD_ON + "Commit-msg hook setup successfully." + COLOR_OFF)

def main():
    # Get arguments

    if "--hooks" not in sys.argv:
        url = getUrl()
        binary = getBinary()
        template = getTemplate()

        # Set variables
        variables = getVariables(url, binary, template)

        # Replace variables in Makefile
        setupMakefile(variables)

        # Replace variables in .gitignore
        setupGitignore(variables)

    # Setup pre-push hook (copy ./.github/c_checker.py to ./.git/hooks/pre-push, or ./.github/cpp_checker.py and chmod +x)
    setupHooks()

if __name__ == "__main__":
    main()
