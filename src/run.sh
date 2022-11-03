path_to_traces="../traces"
executable="./predictor"

echo "=== benchmark with args: $1"

for entry in "$path_to_traces"/*
do
  echo "Executing benchark with $entry..."
  bunzip2 -kc "$path_to_traces/$entry" | $executable $1
  echo "$entry done!"
done

echo "=== benchmark done!"
