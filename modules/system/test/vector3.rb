assert 'Moon::Vector3#initialize' do
  assert_equal [0, 0, 0], Moon::Vector3.new.to_a
  assert_equal [1, 2, 3], Moon::Vector3.new(1, 2, 3).to_a
end

assert 'Moon::Vector3#to_a' do
  v1 = Moon::Vector3.new(1, 2, 3)
  assert_equal [1, 2, 3], v1.to_a
end

assert 'Moon::Vector3#[]' do
  assert_equal [1, 1, 1], Moon::Vector3[1].to_a
  assert_equal [1, 2, 3], Moon::Vector3[[1, 2, 3]].to_a
  assert_equal [1, 2, 3], Moon::Vector3[1, [2, 3]].to_a
  assert_equal [1, 2, 3], Moon::Vector3[[1, 2], 3].to_a
  assert_equal [1, 2, 3], Moon::Vector3[1, 2, 3].to_a
  assert_equal [1, 2, 3], Moon::Vector3[Moon::Vector1.new(1), 2, 3].to_a
  assert_equal [1, 2, 3], Moon::Vector3[1, Moon::Vector2.new(2, 3)].to_a
  assert_equal [1, 2, 3], Moon::Vector3[Moon::Vector1.new(1), Moon::Vector2.new(2, 3)].to_a
  assert_equal [1, 2, 3], Moon::Vector3[Moon::Vector3.new(1, 2, 3)].to_a
end
