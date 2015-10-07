#!/usr/bin/env ruby

# Index is a binary number SWNE, i.e. 10_10 = 1010_2 = vertical line
# LHS: light and rounded; RHS: bold and straight
Boxes = [] 
Boxes[1] = "\u2574" # "\u257a"
Boxes[2] = "\u2575" # "\u2579"
Boxes[3] = "\u2570" # "\u2517"
Boxes[4] = "\u2574" # "\u2578"
Boxes[5] = "\u2500" # "\u2501"
Boxes[6] = "\u256f" # "\u251b"
Boxes[8] = "\u2577" # "\u257b"
Boxes[9] = "\u256d" # "\u250f"
Boxes[10] = "\u2502" # "\u2503"
Boxes[12] = "\u256e" # "\u2513"

def pos_to_xy (pos)
  return nil if @height.nil? || @width.nil?
  return [pos % @width, pos / @width]
end

def read_state (desc)
  puts desc
  desc.chomp!("\n")
  fail "Couldn't parse gameID" unless (m = desc.match /\A([^:]+):([^,]*),([0-9S,]*)\z/)
  n = m[1].match /(\d+)x(\d+)/
  @width = n[1].to_i
  @height = n[2].to_i

  @board = []

  pos = 0
  m[2].split(//).each do |c|
    val = nil
    #print "[#{c}]"
    case c
    when 'a'..'z'
      pos += c.ord - 'a'.ord
    when '1'..'9'
      val = c.to_i
    when 'A'..'Z'
      val = c.ord - 'A'.ord + 10
    end
    #x,y = pos_to_xy(pos)
    #print " (#{pos}:#{x},#{y})"
    #print Boxes[val] if val
    #print "\n"
    @board[pos] = val if val
    pos += 1
  end
  fail 'Clue string too short' unless pos == @height * @width

  counts = m[3].gsub(/S/, '').split ','
  @counts_x = counts[0...@width]
  @counts_y = counts[@width..(@width+@height)]
end

# \u25e6: empty ("white") bullet ("maybe")
# \u2022: solid ("black") bullet ("yes")
# \u00d7: times                  ("no")
def print_board
  puts @counts_x.join ' '
  pos = 0
  @height.times do |y|
    @width.times do |x|
      print (@board[pos]) ? Boxes[@board[pos]] : "\u25e6"
      print ' '
      pos += 1
    end
    print @counts_y[y]
    print "\n"
  end
end

read_state STDIN.readline
print_board
