assert 'Moon::Vector2#initialize' do
  v1 = Moon::Vector2.new
end

assert 'Moon::Vector2#to_a' do
  v1 = Moon::Vector2.new(1, 2)
  assert_equal [1, 2], v1.to_a
end

assert 'Moon::Vector2#[]' do
  assert_equal [1, 1], Moon::Vector2[1].to_a
  assert_equal [1, 2], Moon::Vector2[[1, 2]].to_a
  assert_equal [1, 2], Moon::Vector2[1, 2].to_a
  assert_equal [1, 2], Moon::Vector2[Moon::Vector1.new(1), 2].to_a
  assert_equal [1, 2], Moon::Vector2[1, Moon::Vector1.new(2)].to_a
  assert_equal [1, 2], Moon::Vector2[Moon::Vector1.new(1), Moon::Vector1.new(2)].to_a
  assert_equal [1, 2], Moon::Vector2[Moon::Vector2.new(1, 2)].to_a
end
