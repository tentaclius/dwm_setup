#!/bin/bash

CACHE=~/.cache/applications.cache

if [[ $1 =~ c ]]; then
   # Create cache
   for f in $(find /usr/share/applications /usr/local/share/applications ~/.local/share/applications ~/.nix-profile/share/applications -name '*.desktop'); do
      cat "$f" 2>/dev/null | \
         awk 'BEGIN{name=""; comment=""; exec=""}
              {
                  if (name != "" && comment != "")
                     nextfile;
              }
              /^ *Name=/ {
                  if(name != "") next;
                  sub(/[^=]*/, "");
                  s = gensub(/^[ \t]*([^ \t]*)[ \t]*$/, "\\1", "g");
                  name = substr(s, 2);
              }
              /^ *Comment=/ {
                  if(comment != "") next;
                  sub(/[^=]*/, "");
                  s = gensub(/^[ \t]*([^ \t]*)[ \t]*$/, "\\1", "g");
                  comment = substr(s, 2);
              }
              /^ *Exec=/ {
                  if(exec != "") next;
                  sub(/[^=]*/, "");
                  sub(/%.*/, "");
                  exec=substr($0, 2);
              }
              END {
                  if (name == "") exit(1);
                  printf("%s / %s", name, comment);
              }'
      if [[ $? -eq 0 ]]; then
         printf " / "
         basename "$f"
      fi
   done > "$CACHE"
else
   # Run the program from the cache
   cat "$CACHE" | fzf -i --delimiter "\t" --with-nth=1,2 $(if ! [[ $1 =~ a ]]; then echo --nth=1; fi) | \
      awk --field-separator "\t" '{print $0; system("gtk-launch " $3 " 2>/dev/null")}'
fi
