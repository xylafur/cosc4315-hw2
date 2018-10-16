import os
import shutil
import subprocess

def main():
    os.chdir("testing/testfiles")
    files = os.listdir()
    os.chdir("../../")
    for each in files:
        print("Testing {}".format(each))
        res = subprocess.check_output("./mypython testing/testfiles/{}".format(each),
                                      shell=True).decode('utf-8')
        print()
        

if not os.path.isfile("mypython"):
    print("Need to create mypython and call this from parent dir!")
    exit()

if not os.path.isdir("testing/testfiles"):
    print("THere needs to be a testfiles directory")
    exit()

main()


