#!/usr/bin/perl
use strict;
use warnings;
use File::Basename;
use File::Spec;
use File::Copy::Recursive qw(fcopy rcopy dircopy fmove rmove dirmove);

# Probar user-config.jam posant els using boost

$| = 1;
my $dep_file = ".deps";

sub check_boost_deps{
	my $subdir = shift;
	my %deps;
	print "Checking BOOST deps $subdir\n";
	return %deps if(! -e $subdir."/".$dep_file);
	open(DEPS, $subdir."/".$dep_file) or die $!;
	while(my $dep = <DEPS>){
		chomp($dep);
		$deps{$dep} = "$dep";
	}
	close(DEPS);
	return %deps;
}

sub copy_boost_tools{
	my $boost_path = shift;
	my $dest = shift;
	print "Copying BOOST tools ...";
	fcopy("$boost_path/bootstrap.sh", "$dest/");
	fcopy("$boost_path/bootstrap.bat", "$dest/");
	fcopy("$boost_path/boost-build.jam", "$dest/");
	fcopy("$boost_path/Jamroot", "$dest/");
	dircopy("$boost_path/tools/", "$dest/tools");
	print " Done\n";
}

sub copy_boost_lib{
	my $lib = shift;
	my $boost_path = shift;
	my $dest = shift;
	print "Copying library: $lib ...";
	dircopy("$boost_path/boost/", "$dest/libs/$lib");
	print " Done\n";
}

sub copy_boost_includes{
	my $boost_path = shift;
	my $dest = shift;
	die "BOOST path must be valid! \"$boost_path\"" unless(-f "$boost_path/boost/version.hpp");
	print "Copying BOOST include files...";
	dircopy("$boost_path/boost/", "$dest/boost");
	print " Done\n";
}

sub copy_boost_files{
	my $boost_path = shift;
	my $dest = shift;
	my %libs = @_;
	&copy_boost_tools($boost_path, $dest);
	&copy_boost_includes($boost_path, $dest);
	foreach  my $lib (keys %libs){
		&copy_boost_lib($lib, $boost_path, $dest);
	}
}

sub main{
	my $dirpath = dirname(File::Spec->rel2abs($0));
	die "Error: BOOST installation path must be provides as argument\n\n" if($#ARGV + 1 != 2);
	my $boost_path = $ARGV[0];
	my $dest = $ARGV[1];
	my %deps;
	print "Inpecting $dirpath\n";
	opendir(DIR, $dirpath) or die $!;
	while(my $file = readdir(DIR)){
		next unless (-d "$dirpath/$file");
		next if($file =~ m/^\./);
		my %tmpdeps = check_boost_deps("$dirpath/$file");
		foreach  my $lib (keys %tmpdeps){
			$deps{$lib} = $lib;
		}
		print "Copying $file to $dest/libtools ...\n";
		dircopy($file, "$dest/libtools");
	}
	closedir(DIR);
#		foreach  my $lib (keys %deps){
#			print "$lib\n";
#		}
	mkdir $dest if(! -e $dest);
	&copy_boost_files($boost_path, $dest, %deps);
	return 0;
}

&main();
