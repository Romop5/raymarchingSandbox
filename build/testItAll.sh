function testIt()
{
    echo "Testing: SEED($1) CLUSTERS($2) SAH($3) MAXLEVELS($4)"
    ./bvhgenerator --seed $1 -c $2 -s $3 -l $4 
    ./sandbox --test -i optimized.sdf | grep -v "Frame ID" | tail -n6
}

testIt 1337 1 1.0 0

testIt 1337 1 0.9 1
testIt 1337 1 0.5 1
testIt 1337 1 0.4 1
testIt 1337 1 0.3 1
testIt 1337 1 0.1 1

testIt 1337 1 0.9 2
testIt 1337 1 0.5 2
testIt 1337 1 0.4 2
testIt 1337 1 0.3 2
testIt 1337 1 0.1 2

testIt 1337 1 0.9 3
testIt 1337 1 0.5 3
testIt 1337 1 0.4 3
testIt 1337 1 0.3 3
testIt 1337 1 0.1 3

testIt 887788 1 1.0 0

testIt 887788 1 0.9 1
testIt 887788 1 0.5 1
testIt 887788 1 0.4 1
testIt 887788 1 0.3 1
testIt 887788 1 0.1 1

testIt 887788 1 0.9 2
testIt 887788 1 0.5 2
testIt 887788 1 0.4 2
testIt 887788 1 0.3 2
testIt 887788 1 0.1 2

testIt 887788 1 0.9 3
testIt 887788 1 0.5 3
testIt 887788 1 0.4 3
testIt 887788 1 0.3 3
testIt 887788 1 0.1 3

testIt 136135 1 0.9 0

testIt 136135 1 0.9 1
testIt 136135 1 0.5 1
testIt 136135 1 0.4 1
testIt 136135 1 0.3 1
testIt 136135 1 0.1 1

testIt 136135 1 0.9 2
testIt 136135 1 0.5 2
testIt 136135 1 0.4 2
testIt 136135 1 0.3 2
testIt 136135 1 0.1 2

testIt 136135 1 0.9 3
testIt 136135 1 0.5 3
testIt 136135 1 0.4 3
testIt 136135 1 0.3 3
testIt 136135 1 0.1 3


testIt 1337 2 0.9 0

testIt 1337 2 0.9 1
testIt 1337 2 0.5 1
testIt 1337 2 0.4 1
testIt 1337 2 0.3 1
testIt 1337 2 0.1 1

testIt 1337 2 0.9 2
testIt 1337 2 0.5 2
testIt 1337 2 0.4 2
testIt 1337 2 0.3 2
testIt 1337 2 0.1 2

testIt 1337 2 0.9 3
testIt 1337 2 0.5 3
testIt 1337 2 0.4 3
testIt 1337 2 0.3 3
testIt 1337 2 0.1 3

testIt 887788 2 0.9 0

testIt 887788 2 0.9 1
testIt 887788 2 0.5 1
testIt 887788 2 0.4 1
testIt 887788 2 0.3 1
testIt 887788 2 0.1 1

testIt 887788 2 0.9 2
testIt 887788 2 0.5 2
testIt 887788 2 0.4 2
testIt 887788 2 0.3 2
testIt 887788 2 0.1 2

testIt 887788 2 0.9 3
testIt 887788 2 0.5 3
testIt 887788 2 0.4 3
testIt 887788 2 0.3 3
testIt 887788 2 0.1 3

testIt 136135 2 0.9 0

testIt 136135 2 0.9 1
testIt 136135 2 0.5 1
testIt 136135 2 0.4 1
testIt 136135 2 0.3 1
testIt 136135 2 0.1 1

testIt 136135 2 0.9 2
testIt 136135 2 0.5 2
testIt 136135 2 0.4 2
testIt 136135 2 0.3 2
testIt 136135 2 0.1 2

testIt 136135 2 0.9 3
testIt 136135 2 0.5 3
testIt 136135 2 0.4 3
testIt 136135 2 0.3 3
testIt 136135 2 0.1 3

testIt 1337 3 0.9 0

testIt 1337 3 0.9 1
testIt 1337 3 0.5 1
testIt 1337 3 0.4 1
testIt 1337 3 0.3 1
testIt 1337 3 0.1 1

testIt 1337 3 0.9 2
testIt 1337 3 0.5 2
testIt 1337 3 0.4 2
testIt 1337 3 0.3 2
testIt 1337 3 0.1 2

testIt 1337 3 0.9 3
testIt 1337 3 0.5 3
testIt 1337 3 0.4 3
testIt 1337 3 0.3 3
testIt 1337 3 0.1 3

testIt 887788 3 0.9 0

testIt 887788 3 0.9 1
testIt 887788 3 0.5 1
testIt 887788 3 0.4 1
testIt 887788 3 0.3 1
testIt 887788 3 0.1 1

testIt 887788 3 0.9 2
testIt 887788 3 0.5 2
testIt 887788 3 0.4 2
testIt 887788 3 0.3 2
testIt 887788 3 0.1 2

testIt 887788 3 0.9 3
testIt 887788 3 0.5 3
testIt 887788 3 0.4 3
testIt 887788 3 0.3 3
testIt 887788 3 0.1 3

testIt 136135 3 0.9 0

testIt 136135 3 0.9 1
testIt 136135 3 0.5 1
testIt 136135 3 0.4 1
testIt 136135 3 0.3 1
testIt 136135 3 0.1 1

testIt 136135 3 0.9 2
testIt 136135 3 0.5 2
testIt 136135 3 0.4 2
testIt 136135 3 0.3 2
testIt 136135 3 0.1 2

testIt 136135 3 0.9 3
testIt 136135 3 0.5 3
testIt 136135 3 0.4 3
testIt 136135 3 0.3 3
testIt 136135 3 0.1 3
