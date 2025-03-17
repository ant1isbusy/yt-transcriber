#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <url>"
    exit 1
fi

URL="$1"

./scraper.out "$URL"

python3 summarizer.py

echo "Summary saved to summary.txt"
