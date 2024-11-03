for i in $(seq 1 10); do python cenas_bic.py input/vd$(printf "%02d" $i).mp4 0.6; done
