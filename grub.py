# Liza feat. Lord Osiris
# GrubScript ver 0.2

import sys
import os
import commands
trunk = commands.getoutput("cat .svn/entries | head -n 4 | tail -n 1")
patchlevel = commands.getoutput("cat include/version.h | grep PATCHLEVEL | cut -b 21-23")

if os.getuid() != 0:
	print "[-] You need to run this install script as root!"
	sys.exit(1)

def grub_hd(pn):
	letter = pn[2]
	nump = int(pn[3]) - 1
	alphanum = { 'a':0, 'b':1, 'c':2, 'd':3, 'e':4, 'f':5 }
    	
	res = "(hd" + str(alphanum[letter]) + "," + str(nump) + ")"
	return res

def sub_mount(mp, cwd):
	count = 0
	
	for x in mp:
		if mp[count] == cwd[count]:
			count+=1
	return cwd[count:]

# Manca il controllo errori
fd = open("/boot/grub/menu.lst", "a")

print "------------ DreamOS Grub Autogen --------------"
print "Inserire la partizione dei sorgenti (ex. hda2): ",
pn = raw_input()
partition = grub_hd(pn)

print "Inserire il mountpoint:",
mp = raw_input()
cwd = os.getcwd() + "/dreamos.img"

if mp == "/":
	path = cwd
else:
	cwd = sub_mount(mp, cwd)
	

print "+--------------------------- INFO ------------------------------+"
print "| Title: DreamOS v0." + patchlevel + "-" + trunk +"                                     |"
print "| Root: " + partition + "                                  		|"
print "| Kernel: " + cwd +"	|"
print "+---------------------------------------------------------------+"

print "Procedere alla scrittura? (y/n):",
answer = raw_input()

if answer == 'y':
	fd.write("\ntitle\tDreamOS v0." + patchlevel + "-" + trunk +"")
	fd.write("\nroot\t" + partition)
	fd.write("\nkernel\t" + cwd)
	fd.write("\nboot\n")
	fd.close()
	print ""
        print "Changes added to /boot/grub/menu.lst: "
        print "\n+title\tDreamOS trunk"
        print "+root\t" + partition
        print "+kernel\t" + cwd
        print "+boot\n"
        print "[+] Done."


elif answer == 'n':
	print "Ok, bye :)"

