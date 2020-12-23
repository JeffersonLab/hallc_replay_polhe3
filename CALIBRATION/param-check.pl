#!/usr/bin/perl
#
## NOTES:  This code is ugly and evolved from a 'quick and dirty' hack.  Don't
#          learn from it...
#
# TODO:
#   - Could implement the parameter substitution  that is used in some param files
#     - That would allow checking the 'final' values hcana ends up with
#
# Brad Sawatzky <brads@jlab.org>

use strict;
use warnings;

use Data::Dumper;
use Term::ANSIColor qw(:constants);
use Getopt::Long;
use Scalar::Util qw(looks_like_number);

my $MAXSIG = 10;
my $help;
my $verbose=0;
my @cfiles;

GetOptions(
  "h|help" => \$help,
  "v|verbose+" => \$verbose,
  "m|maxsigma=f" => \$MAXSIG,
  "c|compare=s{2}" => \@cfiles
);
sub usage() {
  print "Usage: $0 <files>

  Parse and compare hcana parameter (PARAM/*.param) files.

  You'll want to run this on a terminal that supports color.

  By default, each file will sanity checked individually.  Parsing
  errors will be flagged, and (array only!) data that contain values
  more than 'maxsigma' ($MAXSIG) std.dev away from the mean will be flagged.

  Add '-v' to print out all parameters in a standardized format.

  Two files (ie. before and after new calibration) can be compared with the
  '-c' option.  Differences are computed and evaluated between parameter
  values.

  ** NOTE: Many flagged outliers are OK.  It is up to /you/ to understand
           what is OK (and why) and what is a problem/error!

  FLAGS:
  -c | --compare    (Requires 2 files as arguments)
  -m | --maxsigma   (Warn if sigma exceeds value [$MAXSIG])
  -v | --verbose
  -h | --help

  \n";
}

if($help) {
  usage();
  exit(0);
}

if( (@cfiles > 0) and (@cfiles != 2) ) {
  print "-!-> '--compare requires 2 filenames as arguments\n\n";
  print "ARGV:   " . Dumper(@ARGV) . "\n";
  print "cfiles: " . Dumper(@cfiles) . "\n";
  exit(1);
}

sub load_file($) {
  my $f = shift;

  print "---> Parsing file: ", BLUE, "\t$f ", RESET, ". . . ";
  open (my $fp, "<$f")
    or die "  -!-> Can't open file '$f': $!\n";

  my %pfile;
  my %params;
  my $comment="";
  my $last_comment="";
  my $p = undef;
  my @vals = ();

  my $last_line="";
  while(my $line = <$fp>) {
    chomp($line);
    if($line =~ s/\s*[#;](.*)\s*$//) { # strip comment
      $comment .= "  $1";
    }
    next if($line =~ /^\s*$/);         # skip empty lines

    $line =~ s/^\s*//;                 # strip leading whitespace
    if($line =~ /([\w]+)\s*=\s*(.*)\s*$/) { # look for <param> = .... line
      my $new_param=$1;
      if( defined($p) ) {                 # store prior param=vals data (if present)
        store_param($p, \@vals, $last_comment, $last_line, \%params);
        $p = undef;
        $last_comment = $comment;
        $comment = "";
        @vals=();                         # clear @vals
      } else {
        if( ! exists( $pfile{comment} ) ) {
          ## Accumulated comment should header be from top of file
          $pfile{comment} = $comment;
          $last_comment   = $comment;
          $comment = "";
        }
      }
      $p=$new_param;
      push( @vals, split_vals($2) );
    } else {
      push( @vals, split_vals($line) );
    }

    $last_line = $line;
  }

  ## Store final parameter
  store_param($p, \@vals, $comment, $last_line, \%params);

  close $fp or die "Can't close file '$f': $!\n";
  $pfile{filename} = $f;
  $pfile{params}   = \%params;
  $pfile{comment} = "" if( ! exists( $pfile{comment} ) );

  print "  Done.\n";

  return(\%pfile);
}

sub store_param($$$$$) {
  my $p         = shift;
  my $val_ref   = shift;
  my $comment   = shift;
  my $last_line = shift;
  my $param_ref = shift;

  if( defined($p) ) {                 # store prior param=vals data (if present)
    if($last_line =~ /,\s*$/) {
      print YELLOW, "\n-!-> WARNING:  trailing comma on last line for param '$p'\n", RESET;
    }
    my ($type, $cleaned_vals) = validate_vals($p, @$val_ref );  # validiate AND clean up @vals (in-situ!)
    $param_ref->{$p}{vals}    = [ @$cleaned_vals ];
    $param_ref->{$p}{type}    = $type;     # store parameter type (string or numeric)
    $param_ref->{$p}{comment} = $comment;  # store parameter description
  }
}

sub split_vals($) {
  ## Attempt to catch the various 'parameter' types scattered through param files...
  my $line=shift;
  my @vals;

  ## All numereric, no math operators (but possible neg. sign)
  ## Assume space delimited, *not* comma delimited... (ie.  hhodo_calib.param files)
  if($line =~ /^[-.\s0-9]+$/) {
    my @tmp =  split(/\s+/, $line);
    my $c=0;
    foreach my $v ( @tmp ) { $c++ if looks_like_number($v); }
    if( $c == scalar(@tmp) ) {  # Confirm all tokens are actually numbers
      push @vals, @tmp;
    } else {                    # if not, strip spaces and assume it is a formula with a minus sign...
      $line =~ s/\s+//g;
      push @vals, $line;
    }
    return @vals;
  }

  ## Generally assume comma delimited
  $line =~ s/,\s*$//;                 # strip trailing comma on line
  foreach my $v (split(/,/, $line)) {
    if($v =~ /["'](.+?)["']/) {       # assume this is quoted string
      push @vals, $1;
      next;
    }

    $v =~ s/\s+//g;                   # strip whitespace
    if($v =~ /[-+.0-9\/\(\)]+/) {     # all numbers or simple formula in first comma-sep 'value'
      push @vals, $v;
    } else {                          # otherwise assume whole line is one arguement/formula
      push @vals, $line;
      last;
    }
  }
  return @vals;
}

sub validate_vals($@) {
  my $param   = shift;

  my @vals;
  my $type;
  my $N_numeric=0;

  my $i=-1;    # so first val is index 0
  foreach ( @_ ) {
    $i++;
    if(looks_like_number($_)) {
      $N_numeric++;
      push @vals, $_;
      next;
    }

    if($_ =~ /^[-+*\s\/.0-9\(\)]+$/) { # Simple algebraic expression (no variables)
      my $v = $_;
      $v =~ s/\s+//g;
      my $tmp = eval( $v );
      if( defined($tmp) ) {
        push @vals, $tmp;
        $N_numeric++;
        next;
      } else {
        print "\n", RED, "-!-> WARNING: Invalid calculated argument for parameter '$param' val[$i]: '$_'\n", RESET;
      }
    }

    push @vals, $_;    # Keep original value as fallback
  }
  if( $N_numeric == scalar(@vals) ) { $type="numeric"; }
  elsif($N_numeric == 0)            { $type="string";  }
  else                              {
    print "\n", RED, "-!-> WARNING: Mix of 'numeric' and 'string' entries for parameter '$param'\n", RESET;
    print "       Values = \"", RED, join(', ', @vals) . "\"\n", RESET;
    $type = "inconsistent";
  }
  return $type, \@vals;
}

sub compare_params($$) {
  my $pf1 = shift;
  my $pf2 = shift;

  my $fn1 = $pf1->{filename};
  my $fn2 = $pf2->{filename};
  my @k1 = sort (keys( %{$pf1->{params}} ));
  my @k2 = sort (keys( %{$pf2->{params}} ));

  print "Comparing parameters in files:\n";
  print "  $fn1\n";
  print "  $fn2\n";
  print "\n";

  #print "Keys: " . join(",  ", @k1) . "\n";

  print "--> Cross check params are all present in each file...\n";
  foreach my $k (@k1) {
    die "-!-> Param '$k' missing in '$fn2'\n" unless ( exists($pf2->{params}->{$k}) );
  }
  foreach my $k (@k2) {
    die "-!-> Param '$k' missing in '$fn1'\n" unless ( exists($pf1->{params}->{$k}) );
  }
  print "    Done.\n\n";

  print "--> Comparing all parameters for each keyword...\n";
  foreach my $k ( @k1 ) {
    my @diffs;
    my @percent_diffs;

    print "----- $k -----\n";
    print $pf1->{params}->{$k}{comment} . "\n";
    my @vals1 = @{$pf1->{params}->{$k}{vals}};
    my @vals2 = @{$pf2->{params}->{$k}{vals}};

    my $type1 = $pf1->{params}->{$k}{type};
    my $type2 = $pf2->{params}->{$k}{type};

    if( $type1 ne $type2 ) {
      print RED;
      printf ("-|-> WARNING: Different value types for parameter (%s vs. %s)\n", $type1, $type2);
      print RESET;
      next;  # skip additional cross-checks on this parameter
    }
    print "---> Parameter type: ", BLUE, "$type1\n", RESET;

    if( $type1 eq "inconsistent") {
      print RED, "-|-> WARNING: parameter type is INCONSISTENT (mix of numeric/string) for parameter '$k'\n", RESET;
      next;
    }

    ## Skip processing on non-numeric parameters
    next if( $type1 ne "numeric");

    if( scalar(@vals1) != scalar(@vals2) ) {
      print RED;
      printf "-|-> WARNING: Different array lengths for parameter (%d vs. %d)\n", scalar(@vals1), scalar(@vals2);
      print RESET;
    }

    my $stats1    = stats_array(@vals1);
    my $stats1_nz = stats_array_suppressZeroes(@vals1);
    my $stats2    = stats_array(@vals2);
    my $stats2_nz = stats_array_suppressZeroes(@vals2);

    foreach ( 0 .. $#vals1 ) {
      my $d = $vals1[$_] - $vals2[$_];
      my $pdiff = $vals1[$_] != 0 ? $d / $vals1[$_] : $d;

      push(@diffs, $d);
      push(@percent_diffs, $pdiff);
    }
    my $diffstats = stats_array(@diffs);

    print "  DIFFERENCES between:  ", BLUE, "$k($fn1)", BLACK " - ", BLUE, "$k($fn2)\n";
    print_stats($diffstats);
    print_table(\@diffs, $diffstats);
    #print_table(\@percent_diffs);         # Prints a table of computed 'percent diffs'; not useful either
    #print_pdiff_table(@percent_diffs);    # Prints a table of 'percent diff' indicators; not super useful in the end

    print "  $k($fn1)\n";
    if(@vals1 > 1) {
      print "    (NOTE on stats:  ", BLUE, "All data in array", RESET, ";  ", MAGENTA, "Ignoring zeroes", RESET, ").\n";
      print BLUE; print_stats($stats1);
      print MAGENTA; print_stats($stats1_nz);
    }
    print_table(\@vals1, $stats1_nz);

    print "  $k($fn2)\n";
    if(@vals2 > 1) {
      print "    (NOTE on stats:  ", BLUE, "All data in array", RESET, ";  ", MAGENTA, "Ignoring zeroes", RESET, ").\n";
      print BLUE; print_stats($stats2);
      print MAGENTA; print_stats($stats2_nz);
    }
    print_table(\@vals2, $stats2_nz);
    print "-"x100 . "\n";
  }

}

sub print_file_info($) {
  my $pf1 = shift;

  my $fn1 = $pf1->{filename};
  my @k1 = sort (keys( %{$pf1->{params}} ));

  my $print_header=0;
  foreach my $k ( @k1 ) {
    my @vals1 = @{$pf1->{params}->{$k}{vals}};
    my $type1 = $pf1->{params}->{$k}{type};
    my $stats1 = undef;
    my $stats1_nz = undef;
    if( $type1 eq "inconsistent") {
      print RED, "-|-> WARNING: parameter type is INCONSISTENT (mix of numeric/string) for parameter '$k'\n", RESET;
    }
    if ($type1 eq "numeric") {
      $stats1 = stats_array(@vals1);
      $stats1_nz = stats_array_suppressZeroes(@vals1);
    }
    if( ($verbose > 0) or ( ($stats1->{stddev} > $MAXSIG) and ($stats1->{stddev} > 0) ) ) {
      if($print_header++ < 1) {
        print "="x100, "\n";
        print "Information for file:", BLUE, "\t$fn1\n", RESET;
        print GREEN, $pf1->{comment} . "\n", RESET;
        print "    (NOTE on stats:  ", BLUE, "All data in array", RESET, ";  ", MAGENTA, "Ignoring zeroes", RESET, ").\n";
      }
      print "----- $k -----\n";
      print GREEN, $pf1->{params}->{$k}{comment} . "\n", RESET;
      if( defined($stats1) and ($stats1->{stddev} > $MAXSIG)) {
        print RED, "-!->  WARNING! :  'Anomalously' large standard deviation [> $MAXSIG] <-!-\n", RESET;
      }
      if( ($type1 eq "numeric") and (@vals1 > 1) ) {
        print BLUE;    print_stats($stats1);
        print MAGENTA; print_stats($stats1_nz);
      }
      print RESET;   print_table(\@vals1, $stats1_nz);
    }
  }
  print "-"x100, "\n" if($print_header);
}

sub stats_array(@) {
  my %stats;

  my $N=scalar(@_);
  my $tmp=0;
  foreach ( @_ ) {
    $tmp+=$_;
  }
  $stats{N}=$N;
  $stats{sum}=$tmp;
  $stats{mean}= $N==0 ? -1 : $tmp/$N;

  $tmp=0;
  foreach ( @_ ) {
    $tmp+= ($_ - $stats{mean})**2
  }
  $stats{stddev}=$N==0 ? -1 : sqrt($tmp/$N);

  return \%stats;
}

sub stats_array_suppressZeroes(@) {
  my @tmp;

  foreach ( @_ ) {
    push @tmp, $_ if($_ != 0);
  }

  return stats_array(@tmp);
}

sub print_stats($) {
  my $href=shift;

  foreach my $k (sort keys(%$href) ) {
    printf("    %s = %8.2f  :", $k, $href->{$k});
  }
  print "\n";
}

sub print_pdiff_table() {
  my $i=0;
  print "  Values:";
  foreach my $p (@_) {
    print "\n   " unless ($i++ % 10);
    if   ( abs($p) <= 0.001)  {print GREEN, "    =     "}
    elsif( abs($p) <= 0.010)  {print YELLOW, "    .     "}
    elsif( abs($p) <= 0.100)  {print MAGENTA, "    x     "}
    else                      {print RED, "    !     "};
  }
  print "\n", RESET;
}

sub print_table() {
  my $vals=shift;
  my $stats=shift;

  my $i=0;
  my $len = scalar(@$vals);
  print "  Values:";
  if( defined($stats) ) {
    print "      [ ", YELLOW, ">1 stddev", RESET, "; ", RED, ">2 stddevs", RESET, " ]" if($len > 1);
  }
  my $stddev = defined($stats->{stddev}) ? $stats->{stddev} : -1;
  foreach my $p (@$vals) {
    print "\n" unless ($i++ % 10);
    my $val;
    if( looks_like_number($p) ) {
      $val = sprintf("% 10.3f", $p);
    } else {
      $val = sprintf("    %s", $p);
    }
    if( $stddev > 0 ) {
      my $d = abs($p - $stats->{mean})/$stddev;
      print YELLOW if($d > 1);
      print RED    if($d > 2);
      print RESET, ON_WHITE  if($p==0);
    }
    print $val, RESET;
  }
  print "\n\n";
}

#######################################################################################

my @pfile_list;
if( @cfiles > 0 ) {
  foreach my $f (@cfiles) {
    my $dat = load_file($f);
    push @pfile_list, $dat;

    #print "\n"; 
    print_file_info($dat);
  }
  print "\n"; compare_params($pfile_list[0], $pfile_list[1]);
} else {
  foreach my $f (@ARGV) {
    my $dat = load_file($f);
    push @pfile_list, $dat;
    print_file_info($dat);
  }
}

