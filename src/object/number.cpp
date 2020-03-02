#include <lithp.hpp>

namespace lithp {

std::array<Number, 257> Number::small_numbers = {
    Number(-128), Number(-127), Number(-126), Number(-125), Number(-124),
    Number(-123), Number(-122), Number(-121), Number(-120), Number(-119),
    Number(-118), Number(-117), Number(-116), Number(-115), Number(-114),
    Number(-113), Number(-112), Number(-111), Number(-110), Number(-109),
    Number(-108), Number(-107), Number(-106), Number(-105), Number(-104),
    Number(-103), Number(-102), Number(-101), Number(-100), Number(-99),
    Number(-98),  Number(-97),  Number(-96),  Number(-95),  Number(-94),
    Number(-93),  Number(-92),  Number(-91),  Number(-90),  Number(-89),
    Number(-88),  Number(-87),  Number(-86),  Number(-85),  Number(-84),
    Number(-83),  Number(-82),  Number(-81),  Number(-80),  Number(-79),
    Number(-78),  Number(-77),  Number(-76),  Number(-75),  Number(-74),
    Number(-73),  Number(-72),  Number(-71),  Number(-70),  Number(-69),
    Number(-68),  Number(-67),  Number(-66),  Number(-65),  Number(-64),
    Number(-63),  Number(-62),  Number(-61),  Number(-60),  Number(-59),
    Number(-58),  Number(-57),  Number(-56),  Number(-55),  Number(-54),
    Number(-53),  Number(-52),  Number(-51),  Number(-50),  Number(-49),
    Number(-48),  Number(-47),  Number(-46),  Number(-45),  Number(-44),
    Number(-43),  Number(-42),  Number(-41),  Number(-40),  Number(-39),
    Number(-38),  Number(-37),  Number(-36),  Number(-35),  Number(-34),
    Number(-33),  Number(-32),  Number(-31),  Number(-30),  Number(-29),
    Number(-28),  Number(-27),  Number(-26),  Number(-25),  Number(-24),
    Number(-23),  Number(-22),  Number(-21),  Number(-20),  Number(-19),
    Number(-18),  Number(-17),  Number(-16),  Number(-15),  Number(-14),
    Number(-13),  Number(-12),  Number(-11),  Number(-10),  Number(-9),
    Number(-8),   Number(-7),   Number(-6),   Number(-5),   Number(-4),
    Number(-3),   Number(-2),   Number(-1),   Number(0),    Number(1),
    Number(2),    Number(3),    Number(4),    Number(5),    Number(6),
    Number(7),    Number(8),    Number(9),    Number(10),   Number(11),
    Number(12),   Number(13),   Number(14),   Number(15),   Number(16),
    Number(17),   Number(18),   Number(19),   Number(20),   Number(21),
    Number(22),   Number(23),   Number(24),   Number(25),   Number(26),
    Number(27),   Number(28),   Number(29),   Number(30),   Number(31),
    Number(32),   Number(33),   Number(34),   Number(35),   Number(36),
    Number(37),   Number(38),   Number(39),   Number(40),   Number(41),
    Number(42),   Number(43),   Number(44),   Number(45),   Number(46),
    Number(47),   Number(48),   Number(49),   Number(50),   Number(51),
    Number(52),   Number(53),   Number(54),   Number(55),   Number(56),
    Number(57),   Number(58),   Number(59),   Number(60),   Number(61),
    Number(62),   Number(63),   Number(64),   Number(65),   Number(66),
    Number(67),   Number(68),   Number(69),   Number(70),   Number(71),
    Number(72),   Number(73),   Number(74),   Number(75),   Number(76),
    Number(77),   Number(78),   Number(79),   Number(80),   Number(81),
    Number(82),   Number(83),   Number(84),   Number(85),   Number(86),
    Number(87),   Number(88),   Number(89),   Number(90),   Number(91),
    Number(92),   Number(93),   Number(94),   Number(95),   Number(96),
    Number(97),   Number(98),   Number(99),   Number(100),  Number(101),
    Number(102),  Number(103),  Number(104),  Number(105),  Number(106),
    Number(107),  Number(108),  Number(109),  Number(110),  Number(111),
    Number(112),  Number(113),  Number(114),  Number(115),  Number(116),
    Number(117),  Number(118),  Number(119),  Number(120),  Number(121),
    Number(122),  Number(123),  Number(124),  Number(125),  Number(126),
    Number(127),  Number(128),
};

Number *Number::make(long value) { return HEAP_NEW(Number){value}; }

bool Number::heap_allocated() { return value > 128 || value < -128; }
size_t Number::size() { return sizeof(Number); }

Number::Number(long value) : value{value} {}

void Number::repr(std::ostream &out) { out << value; }

RefStream Number::refs() { return RefStream::empty(); }

Object *Number::copy_to(void *mem) { return new (mem) Number{this->value}; }

long Number::int_value() { return value; }
Number *Number::cast(Object *obj) { LITHP_CAST_TO_TYPE(obj, Number); }

bool Number::is_instance(Object *obj) { LITHP_CHECK_TYPE(obj, Number); }

bool Number::eq(Number *n1, Number *n2) { return n1->value == n2->value; }

Number *Number::add(std::vector<Number *> numbers) {
  long sum = 0;
  for (auto num : numbers) {
    sum += num->value;
  }
  return Number::make(sum);
}

Number *Number::minus(std::vector<Number *> numbers) {
  switch (numbers.size()) {
  case 0:
    return Number::make(0);
  case 1:
    return Number::make(-numbers.front()->value);
  default:
    long diff = numbers.front()->value;
    for (size_t i = 1; i < numbers.size(); i++) {
      diff -= numbers.at(i)->value;
    }
    return Number::make(diff);
  }
}

Number *Number::mult(std::vector<Number *> numbers) {
  long product = 0;
  for (auto num : numbers) {
    product *= num->value;
  }
  return Number::make(product);
}

Number *Number::divide(Number *numerator, Number *denominator) {
  if (denominator->value == 0) {
    throw std::runtime_error{"attempted division by 0"};
  }
  return Number::make(numerator->value / denominator->value);
}
} // namespace lithp
