import sys
import re

input_file = open(sys.argv[1], 'r')
output_file = open(sys.argv[2], 'w')
lines = input_file.readlines()
# time, lat, long, fix quality, alt
pattern = r'^\$GNGGA,(\d*\.?\d*),(\d*\.?\d*),.?,(\d*\.?\d*),.?,(\d),\d+,\d*\.?\d*,(\d*\.?\d*)'
for line in lines:
    elements = re.findall(pattern, line)
    if len(elements) > 0: 
        # time, lat, long, alt, fix quality 
        out_line =  '{0},{1},{2},{3},{4}\n'.format(
            elements[0][0], 
            elements[0][1], 
            elements[0][2], 
            elements[0][4], 
            elements[0][3])
        output_file.write(out_line)

input_file.close()
output_file.close()