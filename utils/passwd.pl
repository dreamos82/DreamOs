#!/usr/bin/perl -w

use Cwd;

#TODO:
# 1. rendere meno "forzato" e portabile il noecho della richiesta della password.
# 2. flags.
# 3. pulizia generale dello script.

my $__script_version__="0.1";
my $__script_author__="DT";
my $__script_license__="New BSD License";

my $filepath;

if (-e &Cwd::realpath("../files/passwd")) {
	$filepath=&Cwd::realpath("../files/passwd");
} elsif (-e "files/passwd") {
	$filepath="files/passwd";
} else {
	$filepath="passwd";
}

# Read Users

my %passwd = ();
my %junkdata = ();

open(PASSWD, "+<", $filepath) or die("Can't open $filepath: $!");

print "Current Users:\n";
while(<PASSWD>) {
	chomp($_);
	my ($user, $password, @junk) = split(/:/);
	print "$user\n";
	if(@junk) {
		print ".junkdata: @junk\n";
	}
	$passwd{$user}=$password;
	$junkdata{$user}=@junk;
}
print "\n";
# Add User

print "username: ";
$username = <>;
chomp($username);
print "$username\n";

if(!($username =~ m/^(\w+)$/)) {
	print "Invalid username \"$username\"\n";
	exit 1;
}
if(exists $passwd{$username}) {
	print "User \"$username\" exists.\n";
	exit 1;
}



print "password: ";
system("stty -echo");
$password = <>;
system("stty echo");
chomp($password);
print "$password\n";

print "adduser $username:$password\n";

print PASSWD "$username:$password\n";

close(PASSWD);