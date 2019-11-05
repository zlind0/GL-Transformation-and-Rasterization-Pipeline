#!/usr/bin/perl -w

require strict;

my ($dir, $test_dir, $scheme, $token) = @ARGV;

open S, "$scheme" || die "FAIL: could not open grading scheme.";

sub run_test
{
    my ($file,$max_error,$max_time)=@_;
    my $pass_error = 0;
    my $pass_time = 0;
    `rm $dir/file.txt ; cp $test_dir/$file $dir/file.txt`;
    $_=`( cd $dir ; timeout 2 ./minigl -d file.txt || echo TIMEOUT )`;
    if(/TIMEOUT/)
    {
        print "FAIL: ($file) Diff test timed out.\n";
    }
    elsif(/^$token ([0-9.]+)$/)
    {
        if($1+0>$max_error)
        {
            print "FAIL: ($file) Too much error. Actual: $1  Max: $max_error.\n";
        }
        else
        {
            print "PASS: ($file) diff $1 vs $max_error.\n";
            $pass_error = 1;
        }
    }
    else
    {
        print "FAIL: ($file) Did not find diff amount.\n";
    }

    for my $i (1..5)
    {
        $_=`( cd $dir ; timeout 2 ./minigl -t file.txt || echo TIMEOUT )`;
        if(/TIMEOUT/)
        {
            print "FAIL: ($file) Diff test timed out.\n";
        }
        elsif(/^$token ([0-9.]+)$/)
        {
            if($1+0>$max_time)
            {
                print "FAIL: ($file) Too much error. Actual: $1  Max: $max_time.\n";
            }
            else
            {
                print "PASS: ($file) time $1 vs $max_time.\n";
                $pass_time = 1;
            }
        }
        else
        {
            print "FAIL: ($file) Did not find time amount.\n";
        }
        $pass_time && last;
    }
    return $pass_error && $pass_time;
}

my $total_score=0;
while(<S>)
{
    /^\s*\#/ && next;
    /^\s*$/ && next;
    if(/^t\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s*$/)
    {
        my $points=$1;
        my $max_error=$2;
        my $max_time=$3;
        my $file=$4;

        if(&run_test($file,$max_error,$max_time))
        {
            print "+$points points\n";
            $total_score+=$points;
        }
        else
        {
            print "no points\n";
        }
        
        next;
    }
    if(/^g\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+/)
    {
        my $points=$1;
        my $max_error=$2;
        my $max_time=$3;
        my $num_pass=$4;
        my @files=split /\s+/, "$'";

        my $passed = 0;
        for my $file (@files)
        {
            if(&run_test($file,$max_error,$max_time))
            {
                $passed++;
            }
        }
        print "passed $passed of $num_pass required.\n";
        if($passed >= $num_pass)
        {
            print "+$points points\n";
            $total_score+=$points;
        }
        else
        {
            print "no points\n";
        }
        
        next;
    }

    print "Unrecognized command: $_";
}

print "FINAL SCORE: $total_score\n";

