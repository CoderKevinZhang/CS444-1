#!/bin/bash
#Find remote URL for hashes (designed for GitHub-hosted projects)
#Gets my git commit history 
origin=`git config remote.origin.url`
base=`dirname "$origin"`/`basename "$origin" .git`

# Output LaTeX table in chronological order
echo "\\begin{tabular}{l l l}\\textbf{Detail} & \\textbf{Author} & \\textbf{Description}\\\\\\hline"
git log --pretty=format:"\\href{$base/commit/%H}{%h} & %an & %s\\\\\\hline" --reverse
echo "\end{tabular}"

