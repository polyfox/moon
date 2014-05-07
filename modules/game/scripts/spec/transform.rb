puts Vector3[1000, Vector2.new(14, 15)].to_a
puts Vector3[Vector2.new(14, 15), 1000].to_a

puts Vector4[1000, Vector2.new(14, 15), 10].to_a
puts Vector4[Vector2.new(32, 24), Vector2.new(14, 15)].to_a
puts Vector4[Vector3.new(32, 24, 12), 1].to_a
puts Vector4[2, Vector3.new(32, 24, 12)].to_a

t1 = Transform.new
t2 = Transform.new

t3 = t1 * t2
puts t3.to_a.map(&:to_a)
puts t3.to_a16
t4 = t3 + Vector4[12, 32, 24, 1.0]
puts t4[0]
v4 = Vector4[-12, -32, -24, -1.0]
t4[0] += v4

puts t4.to_a.map(&:to_a)
puts (-t4).to_a.map(&:to_a)
t5 = t1.scale(12, 13, 90)
t6 = t1.rotate(45, 1, 2, 3)
puts (t5).to_a.map(&:to_a)
puts (t6).to_a.map(&:to_a)