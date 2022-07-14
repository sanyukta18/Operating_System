#!/bin/bash                                                                                                                                                                                                 

for filename in data1c/*; do
  if [[ -f "$filename" ]]; then
      base=${filename%.*}
      ext=${filename#$base.}
    mkdir -p "${ext}"
    mv "$filename" "${ext}"
  fi
done