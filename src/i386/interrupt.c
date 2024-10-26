#include "interrupt.h"
#include "vga.h"
#include "debug.h"
#include "pic.h"

interrupt_gate_t interrupt_desc_table[256];

interrupt_routine interrupt_handlers[256];

stub_without_error_code(0)
stub_without_error_code(1)
stub_without_error_code(2)
stub_without_error_code(3)
stub_without_error_code(4)
stub_without_error_code(5)
stub_without_error_code(6)
stub_without_error_code(7)
stub_with_error_code(8)
stub_without_error_code(9)
stub_with_error_code(10)
stub_with_error_code(11)
stub_with_error_code(12)
stub_with_error_code(13)
stub_with_error_code(14)
stub_without_error_code(15)
stub_without_error_code(16)
stub_with_error_code(17)
stub_without_error_code(18)
stub_without_error_code(19)
stub_without_error_code(20)
stub_without_error_code(21)
stub_without_error_code(22)
stub_without_error_code(23)
stub_without_error_code(24)
stub_without_error_code(25)
stub_without_error_code(26)
stub_without_error_code(27)
stub_without_error_code(28)
stub_without_error_code(29)
stub_without_error_code(30)
stub_without_error_code(31)
stub_without_error_code(32)
stub_without_error_code(33)
stub_without_error_code(34)
stub_without_error_code(35)
stub_without_error_code(36)
stub_without_error_code(37)
stub_without_error_code(38)
stub_without_error_code(39)
stub_without_error_code(40)
stub_without_error_code(41)
stub_without_error_code(42)
stub_without_error_code(43)
stub_without_error_code(44)
stub_without_error_code(45)
stub_without_error_code(46)
stub_without_error_code(47)
stub_without_error_code(48)
stub_without_error_code(49)
stub_without_error_code(50)
stub_without_error_code(51)
stub_without_error_code(52)
stub_without_error_code(53)
stub_without_error_code(54)
stub_without_error_code(55)
stub_without_error_code(56)
stub_without_error_code(57)
stub_without_error_code(58)
stub_without_error_code(59)
stub_without_error_code(60)
stub_without_error_code(61)
stub_without_error_code(62)
stub_without_error_code(63)
stub_without_error_code(64)
stub_without_error_code(65)
stub_without_error_code(66)
stub_without_error_code(67)
stub_without_error_code(68)
stub_without_error_code(69)
stub_without_error_code(70)
stub_without_error_code(71)
stub_without_error_code(72)
stub_without_error_code(73)
stub_without_error_code(74)
stub_without_error_code(75)
stub_without_error_code(76)
stub_without_error_code(77)
stub_without_error_code(78)
stub_without_error_code(79)
stub_without_error_code(80)
stub_without_error_code(81)
stub_without_error_code(82)
stub_without_error_code(83)
stub_without_error_code(84)
stub_without_error_code(85)
stub_without_error_code(86)
stub_without_error_code(87)
stub_without_error_code(88)
stub_without_error_code(89)
stub_without_error_code(90)
stub_without_error_code(91)
stub_without_error_code(92)
stub_without_error_code(93)
stub_without_error_code(94)
stub_without_error_code(95)
stub_without_error_code(96)
stub_without_error_code(97)
stub_without_error_code(98)
stub_without_error_code(99)
stub_without_error_code(100)
stub_without_error_code(101)
stub_without_error_code(102)
stub_without_error_code(103)
stub_without_error_code(104)
stub_without_error_code(105)
stub_without_error_code(106)
stub_without_error_code(107)
stub_without_error_code(108)
stub_without_error_code(109)
stub_without_error_code(110)
stub_without_error_code(111)
stub_without_error_code(112)
stub_without_error_code(113)
stub_without_error_code(114)
stub_without_error_code(115)
stub_without_error_code(116)
stub_without_error_code(117)
stub_without_error_code(118)
stub_without_error_code(119)
stub_without_error_code(120)
stub_without_error_code(121)
stub_without_error_code(122)
stub_without_error_code(123)
stub_without_error_code(124)
stub_without_error_code(125)
stub_without_error_code(126)
stub_without_error_code(127)
stub_without_error_code(128)
stub_without_error_code(129)
stub_without_error_code(130)
stub_without_error_code(131)
stub_without_error_code(132)
stub_without_error_code(133)
stub_without_error_code(134)
stub_without_error_code(135)
stub_without_error_code(136)
stub_without_error_code(137)
stub_without_error_code(138)
stub_without_error_code(139)
stub_without_error_code(140)
stub_without_error_code(141)
stub_without_error_code(142)
stub_without_error_code(143)
stub_without_error_code(144)
stub_without_error_code(145)
stub_without_error_code(146)
stub_without_error_code(147)
stub_without_error_code(148)
stub_without_error_code(149)
stub_without_error_code(150)
stub_without_error_code(151)
stub_without_error_code(152)
stub_without_error_code(153)
stub_without_error_code(154)
stub_without_error_code(155)
stub_without_error_code(156)
stub_without_error_code(157)
stub_without_error_code(158)
stub_without_error_code(159)
stub_without_error_code(160)
stub_without_error_code(161)
stub_without_error_code(162)
stub_without_error_code(163)
stub_without_error_code(164)
stub_without_error_code(165)
stub_without_error_code(166)
stub_without_error_code(167)
stub_without_error_code(168)
stub_without_error_code(169)
stub_without_error_code(170)
stub_without_error_code(171)
stub_without_error_code(172)
stub_without_error_code(173)
stub_without_error_code(174)
stub_without_error_code(175)
stub_without_error_code(176)
stub_without_error_code(177)
stub_without_error_code(178)
stub_without_error_code(179)
stub_without_error_code(180)
stub_without_error_code(181)
stub_without_error_code(182)
stub_without_error_code(183)
stub_without_error_code(184)
stub_without_error_code(185)
stub_without_error_code(186)
stub_without_error_code(187)
stub_without_error_code(188)
stub_without_error_code(189)
stub_without_error_code(190)
stub_without_error_code(191)
stub_without_error_code(192)
stub_without_error_code(193)
stub_without_error_code(194)
stub_without_error_code(195)
stub_without_error_code(196)
stub_without_error_code(197)
stub_without_error_code(198)
stub_without_error_code(199)
stub_without_error_code(200)
stub_without_error_code(201)
stub_without_error_code(202)
stub_without_error_code(203)
stub_without_error_code(204)
stub_without_error_code(205)
stub_without_error_code(206)
stub_without_error_code(207)
stub_without_error_code(208)
stub_without_error_code(209)
stub_without_error_code(210)
stub_without_error_code(211)
stub_without_error_code(212)
stub_without_error_code(213)
stub_without_error_code(214)
stub_without_error_code(215)
stub_without_error_code(216)
stub_without_error_code(217)
stub_without_error_code(218)
stub_without_error_code(219)
stub_without_error_code(220)
stub_without_error_code(221)
stub_without_error_code(222)
stub_without_error_code(223)
stub_without_error_code(224)
stub_without_error_code(225)
stub_without_error_code(226)
stub_without_error_code(227)
stub_without_error_code(228)
stub_without_error_code(229)
stub_without_error_code(230)
stub_without_error_code(231)
stub_without_error_code(232)
stub_without_error_code(233)
stub_without_error_code(234)
stub_without_error_code(235)
stub_without_error_code(236)
stub_without_error_code(237)
stub_without_error_code(238)
stub_without_error_code(239)
stub_without_error_code(240)
stub_without_error_code(241)
stub_without_error_code(242)
stub_without_error_code(243)
stub_without_error_code(244)
stub_without_error_code(245)
stub_without_error_code(246)
stub_without_error_code(247)
stub_without_error_code(248)
stub_without_error_code(249)
stub_without_error_code(250)
stub_without_error_code(251)
stub_without_error_code(252)
stub_without_error_code(253)
stub_without_error_code(254)
stub_without_error_code(255)

__attribute__((naked))
void interrupt_common_stub()
{
    asm volatile("pusha\n"
                 "push  %%ds\n"
                 "push  %%es\n"
                 "push  %%fs\n"
                 "push  %%gs\n"
                 "mov %%cr3, %%eax\n"
                 "push %%eax\n"
                 "mov   $0x10, %%ax\n"
                 "mov   %%ax, %%ds\n"
                 "mov   %%ax, %%es\n"
                 "mov   %%ax, %%fs\n"
                 "mov   %%ax, %%gs\n"
                 "mov   %%esp, %%eax\n"
                 "push  %%eax\n"
                 "call interrupt_handler\n"
                 "mov   %%eax, %%esp\n" ::);

    asm volatile(
        "pop   %%eax\n"
        "mov   %%cr3, %%ebx\n"
        "cmp   %%eax, %%ebx\n"
        "je    1f\n"
        "mov   %%eax, %%cr3\n"
        "1:\n"
        "pop   %%gs\n"
        "pop   %%fs\n"
        "pop   %%es\n"
        "pop   %%ds\n"
        "popa\n"::);

    asm volatile("add $0x8, %%esp\n"
                 "iret\n" ::);
}

static uint32_t irq_handler(cpu_context_t *context) {
    const uint32_t idx = context->interrupt_number - 32;
    if(!pic_spurious(idx)) {
        interrupt_routine handler = interrupt_handlers[context->interrupt_number];
        if(!handler) panic("unhandled irq: %u\n", idx);
        context = (cpu_context_t *)handler(context);
        pic_write_eoi(idx);
    }
    return (uint32_t)context;
}

static uint32_t isr_handler(cpu_context_t *context) {
    interrupt_routine handler = interrupt_handlers[context->interrupt_number];
    if(!handler) panic("unhandled isr: %u\n", context->interrupt_number);
    return handler(context);
}

unsigned int interrupt_handler(cpu_context_t *context) {
    if(context->interrupt_number >= 32) return irq_handler(context);
    return isr_handler(context);
}

void interrupts_init_isrs() {
    segment_selector_t selector = (segment_selector_t){ .index = 1, .requested_privilege_level = 0, .table_indicator = 0 };
    segment_selector_t selector_user = (segment_selector_t){ .index = 1, .requested_privilege_level = 3, .table_indicator = 0 };


    interrupt_desc_table[0] = interrupt_gate_init((uint32_t)stub_0, selector, 0);
    interrupt_desc_table[1] = interrupt_gate_init((uint32_t)stub_1, selector, 0);
    interrupt_desc_table[2] = interrupt_gate_init((uint32_t)stub_2, selector, 0);
    interrupt_desc_table[3] = interrupt_gate_init((uint32_t)stub_3, selector, 0);
    interrupt_desc_table[4] = interrupt_gate_init((uint32_t)stub_4, selector, 0);
    interrupt_desc_table[5] = interrupt_gate_init((uint32_t)stub_5, selector, 0);
    interrupt_desc_table[6] = interrupt_gate_init((uint32_t)stub_6, selector, 0);
    interrupt_desc_table[7] = interrupt_gate_init((uint32_t)stub_7, selector, 0);
    interrupt_desc_table[8] = interrupt_gate_init((uint32_t)stub_8, selector, 0);
    interrupt_desc_table[9] = interrupt_gate_init((uint32_t)stub_9, selector, 0);
    interrupt_desc_table[10] = interrupt_gate_init((uint32_t)stub_10, selector, 0);
    interrupt_desc_table[11] = interrupt_gate_init((uint32_t)stub_11, selector, 0);
    interrupt_desc_table[12] = interrupt_gate_init((uint32_t)stub_12, selector, 0);
    interrupt_desc_table[13] = interrupt_gate_init((uint32_t)stub_13, selector, 0);
    interrupt_desc_table[14] = interrupt_gate_init((uint32_t)stub_14, selector, 0);
    interrupt_desc_table[15] = interrupt_gate_init((uint32_t)stub_15, selector, 0);
    interrupt_desc_table[16] = interrupt_gate_init((uint32_t)stub_16, selector, 0);
    interrupt_desc_table[17] = interrupt_gate_init((uint32_t)stub_17, selector, 0);
    interrupt_desc_table[18] = interrupt_gate_init((uint32_t)stub_18, selector, 0);
    interrupt_desc_table[19] = interrupt_gate_init((uint32_t)stub_19, selector, 0);
    interrupt_desc_table[20] = interrupt_gate_init((uint32_t)stub_20, selector, 0);
    interrupt_desc_table[21] = interrupt_gate_init((uint32_t)stub_21, selector, 0);
    interrupt_desc_table[22] = interrupt_gate_init((uint32_t)stub_22, selector, 0);
    interrupt_desc_table[23] = interrupt_gate_init((uint32_t)stub_23, selector, 0);
    interrupt_desc_table[24] = interrupt_gate_init((uint32_t)stub_24, selector, 0);
    interrupt_desc_table[25] = interrupt_gate_init((uint32_t)stub_25, selector, 0);
    interrupt_desc_table[26] = interrupt_gate_init((uint32_t)stub_26, selector, 0);
    interrupt_desc_table[27] = interrupt_gate_init((uint32_t)stub_27, selector, 0);
    interrupt_desc_table[28] = interrupt_gate_init((uint32_t)stub_28, selector, 0);
    interrupt_desc_table[29] = interrupt_gate_init((uint32_t)stub_29, selector, 0);
    interrupt_desc_table[30] = interrupt_gate_init((uint32_t)stub_30, selector, 0);
    interrupt_desc_table[31] = interrupt_gate_init((uint32_t)stub_31, selector, 0);
    interrupt_desc_table[32] = interrupt_gate_init((uint32_t)stub_32, selector, 0);
    interrupt_desc_table[33] = interrupt_gate_init((uint32_t)stub_33, selector, 0);
    interrupt_desc_table[34] = interrupt_gate_init((uint32_t)stub_34, selector, 0);
    interrupt_desc_table[35] = interrupt_gate_init((uint32_t)stub_35, selector, 0);
    interrupt_desc_table[36] = interrupt_gate_init((uint32_t)stub_36, selector, 0);
    interrupt_desc_table[37] = interrupt_gate_init((uint32_t)stub_37, selector, 0);
    interrupt_desc_table[38] = interrupt_gate_init((uint32_t)stub_38, selector, 0);
    interrupt_desc_table[39] = interrupt_gate_init((uint32_t)stub_39, selector, 0);
    interrupt_desc_table[40] = interrupt_gate_init((uint32_t)stub_40, selector, 0);
    interrupt_desc_table[41] = interrupt_gate_init((uint32_t)stub_41, selector, 0);
    interrupt_desc_table[42] = interrupt_gate_init((uint32_t)stub_42, selector, 0);
    interrupt_desc_table[43] = interrupt_gate_init((uint32_t)stub_43, selector, 0);
    interrupt_desc_table[44] = interrupt_gate_init((uint32_t)stub_44, selector, 0);
    interrupt_desc_table[45] = interrupt_gate_init((uint32_t)stub_45, selector, 0);
    interrupt_desc_table[46] = interrupt_gate_init((uint32_t)stub_46, selector, 0);
    interrupt_desc_table[47] = interrupt_gate_init((uint32_t)stub_47, selector, 0);
    interrupt_desc_table[48] = interrupt_gate_init((uint32_t)stub_48, selector, 0);
    interrupt_desc_table[49] = interrupt_gate_init((uint32_t)stub_49, selector, 0);
    interrupt_desc_table[50] = interrupt_gate_init((uint32_t)stub_50, selector, 0);
    interrupt_desc_table[51] = interrupt_gate_init((uint32_t)stub_51, selector, 0);
    interrupt_desc_table[52] = interrupt_gate_init((uint32_t)stub_52, selector, 0);
    interrupt_desc_table[53] = interrupt_gate_init((uint32_t)stub_53, selector, 0);
    interrupt_desc_table[54] = interrupt_gate_init((uint32_t)stub_54, selector, 0);
    interrupt_desc_table[55] = interrupt_gate_init((uint32_t)stub_55, selector, 0);
    interrupt_desc_table[56] = interrupt_gate_init((uint32_t)stub_56, selector, 0);
    interrupt_desc_table[57] = interrupt_gate_init((uint32_t)stub_57, selector, 0);
    interrupt_desc_table[58] = interrupt_gate_init((uint32_t)stub_58, selector, 0);
    interrupt_desc_table[59] = interrupt_gate_init((uint32_t)stub_59, selector, 0);
    interrupt_desc_table[60] = interrupt_gate_init((uint32_t)stub_60, selector, 0);
    interrupt_desc_table[61] = interrupt_gate_init((uint32_t)stub_61, selector, 0);
    interrupt_desc_table[62] = interrupt_gate_init((uint32_t)stub_62, selector, 0);
    interrupt_desc_table[63] = interrupt_gate_init((uint32_t)stub_63, selector, 0);
    interrupt_desc_table[64] = interrupt_gate_init((uint32_t)stub_64, selector, 0);
    interrupt_desc_table[65] = interrupt_gate_init((uint32_t)stub_65, selector, 0);
    interrupt_desc_table[66] = interrupt_gate_init((uint32_t)stub_66, selector, 0);
    interrupt_desc_table[67] = interrupt_gate_init((uint32_t)stub_67, selector, 0);
    interrupt_desc_table[68] = interrupt_gate_init((uint32_t)stub_68, selector, 0);
    interrupt_desc_table[69] = interrupt_gate_init((uint32_t)stub_69, selector, 0);
    interrupt_desc_table[70] = interrupt_gate_init((uint32_t)stub_70, selector, 0);
    interrupt_desc_table[71] = interrupt_gate_init((uint32_t)stub_71, selector, 0);
    interrupt_desc_table[72] = interrupt_gate_init((uint32_t)stub_72, selector, 0);
    interrupt_desc_table[73] = interrupt_gate_init((uint32_t)stub_73, selector, 0);
    interrupt_desc_table[74] = interrupt_gate_init((uint32_t)stub_74, selector, 0);
    interrupt_desc_table[75] = interrupt_gate_init((uint32_t)stub_75, selector, 0);
    interrupt_desc_table[76] = interrupt_gate_init((uint32_t)stub_76, selector, 0);
    interrupt_desc_table[77] = interrupt_gate_init((uint32_t)stub_77, selector, 0);
    interrupt_desc_table[78] = interrupt_gate_init((uint32_t)stub_78, selector, 0);
    interrupt_desc_table[79] = interrupt_gate_init((uint32_t)stub_79, selector, 0);
    interrupt_desc_table[80] = interrupt_gate_init((uint32_t)stub_80, selector, 0);
    interrupt_desc_table[81] = interrupt_gate_init((uint32_t)stub_81, selector, 0);
    interrupt_desc_table[82] = interrupt_gate_init((uint32_t)stub_82, selector, 0);
    interrupt_desc_table[83] = interrupt_gate_init((uint32_t)stub_83, selector, 0);
    interrupt_desc_table[84] = interrupt_gate_init((uint32_t)stub_84, selector, 0);
    interrupt_desc_table[85] = interrupt_gate_init((uint32_t)stub_85, selector, 0);
    interrupt_desc_table[86] = interrupt_gate_init((uint32_t)stub_86, selector, 0);
    interrupt_desc_table[87] = interrupt_gate_init((uint32_t)stub_87, selector, 0);
    interrupt_desc_table[88] = interrupt_gate_init((uint32_t)stub_88, selector, 0);
    interrupt_desc_table[89] = interrupt_gate_init((uint32_t)stub_89, selector, 0);
    interrupt_desc_table[90] = interrupt_gate_init((uint32_t)stub_90, selector, 0);
    interrupt_desc_table[91] = interrupt_gate_init((uint32_t)stub_91, selector, 0);
    interrupt_desc_table[92] = interrupt_gate_init((uint32_t)stub_92, selector, 0);
    interrupt_desc_table[93] = interrupt_gate_init((uint32_t)stub_93, selector, 0);
    interrupt_desc_table[94] = interrupt_gate_init((uint32_t)stub_94, selector, 0);
    interrupt_desc_table[95] = interrupt_gate_init((uint32_t)stub_95, selector, 0);
    interrupt_desc_table[96] = interrupt_gate_init((uint32_t)stub_96, selector, 0);
    interrupt_desc_table[97] = interrupt_gate_init((uint32_t)stub_97, selector, 0);
    interrupt_desc_table[98] = interrupt_gate_init((uint32_t)stub_98, selector, 0);
    interrupt_desc_table[99] = interrupt_gate_init((uint32_t)stub_99, selector, 0);
    interrupt_desc_table[100] = interrupt_gate_init((uint32_t)stub_100, selector, 0);
    interrupt_desc_table[101] = interrupt_gate_init((uint32_t)stub_101, selector, 0);
    interrupt_desc_table[102] = interrupt_gate_init((uint32_t)stub_102, selector, 0);
    interrupt_desc_table[103] = interrupt_gate_init((uint32_t)stub_103, selector, 0);
    interrupt_desc_table[104] = interrupt_gate_init((uint32_t)stub_104, selector, 0);
    interrupt_desc_table[105] = interrupt_gate_init((uint32_t)stub_105, selector, 0);
    interrupt_desc_table[106] = interrupt_gate_init((uint32_t)stub_106, selector, 0);
    interrupt_desc_table[107] = interrupt_gate_init((uint32_t)stub_107, selector, 0);
    interrupt_desc_table[108] = interrupt_gate_init((uint32_t)stub_108, selector, 0);
    interrupt_desc_table[109] = interrupt_gate_init((uint32_t)stub_109, selector, 0);
    interrupt_desc_table[110] = interrupt_gate_init((uint32_t)stub_110, selector, 0);
    interrupt_desc_table[111] = interrupt_gate_init((uint32_t)stub_111, selector, 0);
    interrupt_desc_table[112] = interrupt_gate_init((uint32_t)stub_112, selector, 0);
    interrupt_desc_table[113] = interrupt_gate_init((uint32_t)stub_113, selector, 0);
    interrupt_desc_table[114] = interrupt_gate_init((uint32_t)stub_114, selector, 0);
    interrupt_desc_table[115] = interrupt_gate_init((uint32_t)stub_115, selector, 0);
    interrupt_desc_table[116] = interrupt_gate_init((uint32_t)stub_116, selector, 0);
    interrupt_desc_table[117] = interrupt_gate_init((uint32_t)stub_117, selector, 0);
    interrupt_desc_table[118] = interrupt_gate_init((uint32_t)stub_118, selector, 0);
    interrupt_desc_table[119] = interrupt_gate_init((uint32_t)stub_119, selector, 0);
    interrupt_desc_table[120] = interrupt_gate_init((uint32_t)stub_120, selector, 0);
    interrupt_desc_table[121] = interrupt_gate_init((uint32_t)stub_121, selector, 0);
    interrupt_desc_table[122] = interrupt_gate_init((uint32_t)stub_122, selector, 0);
    interrupt_desc_table[123] = interrupt_gate_init((uint32_t)stub_123, selector, 0);
    interrupt_desc_table[124] = interrupt_gate_init((uint32_t)stub_124, selector, 0);
    interrupt_desc_table[125] = interrupt_gate_init((uint32_t)stub_125, selector, 0);
    interrupt_desc_table[126] = interrupt_gate_init((uint32_t)stub_126, selector, 0);
    interrupt_desc_table[127] = interrupt_gate_init((uint32_t)stub_127, selector, 0);

    interrupt_desc_table[128] = interrupt_gate_init((uint32_t)stub_128, selector_user, 0);

    interrupt_desc_table[129] = interrupt_gate_init((uint32_t)stub_129, selector, 0);
    interrupt_desc_table[130] = interrupt_gate_init((uint32_t)stub_130, selector, 0);
    interrupt_desc_table[131] = interrupt_gate_init((uint32_t)stub_131, selector, 0);
    interrupt_desc_table[132] = interrupt_gate_init((uint32_t)stub_132, selector, 0);
    interrupt_desc_table[133] = interrupt_gate_init((uint32_t)stub_133, selector, 0);
    interrupt_desc_table[134] = interrupt_gate_init((uint32_t)stub_134, selector, 0);
    interrupt_desc_table[135] = interrupt_gate_init((uint32_t)stub_135, selector, 0);
    interrupt_desc_table[136] = interrupt_gate_init((uint32_t)stub_136, selector, 0);
    interrupt_desc_table[137] = interrupt_gate_init((uint32_t)stub_137, selector, 0);
    interrupt_desc_table[138] = interrupt_gate_init((uint32_t)stub_138, selector, 0);
    interrupt_desc_table[139] = interrupt_gate_init((uint32_t)stub_139, selector, 0);
    interrupt_desc_table[140] = interrupt_gate_init((uint32_t)stub_140, selector, 0);
    interrupt_desc_table[141] = interrupt_gate_init((uint32_t)stub_141, selector, 0);
    interrupt_desc_table[142] = interrupt_gate_init((uint32_t)stub_142, selector, 0);
    interrupt_desc_table[143] = interrupt_gate_init((uint32_t)stub_143, selector, 0);
    interrupt_desc_table[144] = interrupt_gate_init((uint32_t)stub_144, selector, 0);
    interrupt_desc_table[145] = interrupt_gate_init((uint32_t)stub_145, selector, 0);
    interrupt_desc_table[146] = interrupt_gate_init((uint32_t)stub_146, selector, 0);
    interrupt_desc_table[147] = interrupt_gate_init((uint32_t)stub_147, selector, 0);
    interrupt_desc_table[148] = interrupt_gate_init((uint32_t)stub_148, selector, 0);
    interrupt_desc_table[149] = interrupt_gate_init((uint32_t)stub_149, selector, 0);
    interrupt_desc_table[150] = interrupt_gate_init((uint32_t)stub_150, selector, 0);
    interrupt_desc_table[151] = interrupt_gate_init((uint32_t)stub_151, selector, 0);
    interrupt_desc_table[152] = interrupt_gate_init((uint32_t)stub_152, selector, 0);
    interrupt_desc_table[153] = interrupt_gate_init((uint32_t)stub_153, selector, 0);
    interrupt_desc_table[154] = interrupt_gate_init((uint32_t)stub_154, selector, 0);
    interrupt_desc_table[155] = interrupt_gate_init((uint32_t)stub_155, selector, 0);
    interrupt_desc_table[156] = interrupt_gate_init((uint32_t)stub_156, selector, 0);
    interrupt_desc_table[157] = interrupt_gate_init((uint32_t)stub_157, selector, 0);
    interrupt_desc_table[158] = interrupt_gate_init((uint32_t)stub_158, selector, 0);
    interrupt_desc_table[159] = interrupt_gate_init((uint32_t)stub_159, selector, 0);
    interrupt_desc_table[160] = interrupt_gate_init((uint32_t)stub_160, selector, 0);
    interrupt_desc_table[161] = interrupt_gate_init((uint32_t)stub_161, selector, 0);
    interrupt_desc_table[162] = interrupt_gate_init((uint32_t)stub_162, selector, 0);
    interrupt_desc_table[163] = interrupt_gate_init((uint32_t)stub_163, selector, 0);
    interrupt_desc_table[164] = interrupt_gate_init((uint32_t)stub_164, selector, 0);
    interrupt_desc_table[165] = interrupt_gate_init((uint32_t)stub_165, selector, 0);
    interrupt_desc_table[166] = interrupt_gate_init((uint32_t)stub_166, selector, 0);
    interrupt_desc_table[167] = interrupt_gate_init((uint32_t)stub_167, selector, 0);
    interrupt_desc_table[168] = interrupt_gate_init((uint32_t)stub_168, selector, 0);
    interrupt_desc_table[169] = interrupt_gate_init((uint32_t)stub_169, selector, 0);
    interrupt_desc_table[170] = interrupt_gate_init((uint32_t)stub_170, selector, 0);
    interrupt_desc_table[171] = interrupt_gate_init((uint32_t)stub_171, selector, 0);
    interrupt_desc_table[172] = interrupt_gate_init((uint32_t)stub_172, selector, 0);
    interrupt_desc_table[173] = interrupt_gate_init((uint32_t)stub_173, selector, 0);
    interrupt_desc_table[174] = interrupt_gate_init((uint32_t)stub_174, selector, 0);
    interrupt_desc_table[175] = interrupt_gate_init((uint32_t)stub_175, selector, 0);
    interrupt_desc_table[176] = interrupt_gate_init((uint32_t)stub_176, selector, 0);
    interrupt_desc_table[177] = interrupt_gate_init((uint32_t)stub_177, selector, 0);
    interrupt_desc_table[178] = interrupt_gate_init((uint32_t)stub_178, selector, 0);
    interrupt_desc_table[179] = interrupt_gate_init((uint32_t)stub_179, selector, 0);
    interrupt_desc_table[180] = interrupt_gate_init((uint32_t)stub_180, selector, 0);
    interrupt_desc_table[181] = interrupt_gate_init((uint32_t)stub_181, selector, 0);
    interrupt_desc_table[182] = interrupt_gate_init((uint32_t)stub_182, selector, 0);
    interrupt_desc_table[183] = interrupt_gate_init((uint32_t)stub_183, selector, 0);
    interrupt_desc_table[184] = interrupt_gate_init((uint32_t)stub_184, selector, 0);
    interrupt_desc_table[185] = interrupt_gate_init((uint32_t)stub_185, selector, 0);
    interrupt_desc_table[186] = interrupt_gate_init((uint32_t)stub_186, selector, 0);
    interrupt_desc_table[187] = interrupt_gate_init((uint32_t)stub_187, selector, 0);
    interrupt_desc_table[188] = interrupt_gate_init((uint32_t)stub_188, selector, 0);
    interrupt_desc_table[189] = interrupt_gate_init((uint32_t)stub_189, selector, 0);
    interrupt_desc_table[190] = interrupt_gate_init((uint32_t)stub_190, selector, 0);
    interrupt_desc_table[191] = interrupt_gate_init((uint32_t)stub_191, selector, 0);
    interrupt_desc_table[192] = interrupt_gate_init((uint32_t)stub_192, selector, 0);
    interrupt_desc_table[193] = interrupt_gate_init((uint32_t)stub_193, selector, 0);
    interrupt_desc_table[194] = interrupt_gate_init((uint32_t)stub_194, selector, 0);
    interrupt_desc_table[195] = interrupt_gate_init((uint32_t)stub_195, selector, 0);
    interrupt_desc_table[196] = interrupt_gate_init((uint32_t)stub_196, selector, 0);
    interrupt_desc_table[197] = interrupt_gate_init((uint32_t)stub_197, selector, 0);
    interrupt_desc_table[198] = interrupt_gate_init((uint32_t)stub_198, selector, 0);
    interrupt_desc_table[199] = interrupt_gate_init((uint32_t)stub_199, selector, 0);
    interrupt_desc_table[200] = interrupt_gate_init((uint32_t)stub_200, selector, 0);
    interrupt_desc_table[201] = interrupt_gate_init((uint32_t)stub_201, selector, 0);
    interrupt_desc_table[202] = interrupt_gate_init((uint32_t)stub_202, selector, 0);
    interrupt_desc_table[203] = interrupt_gate_init((uint32_t)stub_203, selector, 0);
    interrupt_desc_table[204] = interrupt_gate_init((uint32_t)stub_204, selector, 0);
    interrupt_desc_table[205] = interrupt_gate_init((uint32_t)stub_205, selector, 0);
    interrupt_desc_table[206] = interrupt_gate_init((uint32_t)stub_206, selector, 0);
    interrupt_desc_table[207] = interrupt_gate_init((uint32_t)stub_207, selector, 0);
    interrupt_desc_table[208] = interrupt_gate_init((uint32_t)stub_208, selector, 0);
    interrupt_desc_table[209] = interrupt_gate_init((uint32_t)stub_209, selector, 0);
    interrupt_desc_table[210] = interrupt_gate_init((uint32_t)stub_210, selector, 0);
    interrupt_desc_table[211] = interrupt_gate_init((uint32_t)stub_211, selector, 0);
    interrupt_desc_table[212] = interrupt_gate_init((uint32_t)stub_212, selector, 0);
    interrupt_desc_table[213] = interrupt_gate_init((uint32_t)stub_213, selector, 0);
    interrupt_desc_table[214] = interrupt_gate_init((uint32_t)stub_214, selector, 0);
    interrupt_desc_table[215] = interrupt_gate_init((uint32_t)stub_215, selector, 0);
    interrupt_desc_table[216] = interrupt_gate_init((uint32_t)stub_216, selector, 0);
    interrupt_desc_table[217] = interrupt_gate_init((uint32_t)stub_217, selector, 0);
    interrupt_desc_table[218] = interrupt_gate_init((uint32_t)stub_218, selector, 0);
    interrupt_desc_table[219] = interrupt_gate_init((uint32_t)stub_219, selector, 0);
    interrupt_desc_table[220] = interrupt_gate_init((uint32_t)stub_220, selector, 0);
    interrupt_desc_table[221] = interrupt_gate_init((uint32_t)stub_221, selector, 0);
    interrupt_desc_table[222] = interrupt_gate_init((uint32_t)stub_222, selector, 0);
    interrupt_desc_table[223] = interrupt_gate_init((uint32_t)stub_223, selector, 0);
    interrupt_desc_table[224] = interrupt_gate_init((uint32_t)stub_224, selector, 0);
    interrupt_desc_table[225] = interrupt_gate_init((uint32_t)stub_225, selector, 0);
    interrupt_desc_table[226] = interrupt_gate_init((uint32_t)stub_226, selector, 0);
    interrupt_desc_table[227] = interrupt_gate_init((uint32_t)stub_227, selector, 0);
    interrupt_desc_table[228] = interrupt_gate_init((uint32_t)stub_228, selector, 0);
    interrupt_desc_table[229] = interrupt_gate_init((uint32_t)stub_229, selector, 0);
    interrupt_desc_table[230] = interrupt_gate_init((uint32_t)stub_230, selector, 0);
    interrupt_desc_table[231] = interrupt_gate_init((uint32_t)stub_231, selector, 0);
    interrupt_desc_table[232] = interrupt_gate_init((uint32_t)stub_232, selector, 0);
    interrupt_desc_table[233] = interrupt_gate_init((uint32_t)stub_233, selector, 0);
    interrupt_desc_table[234] = interrupt_gate_init((uint32_t)stub_234, selector, 0);
    interrupt_desc_table[235] = interrupt_gate_init((uint32_t)stub_235, selector, 0);
    interrupt_desc_table[236] = interrupt_gate_init((uint32_t)stub_236, selector, 0);
    interrupt_desc_table[237] = interrupt_gate_init((uint32_t)stub_237, selector, 0);
    interrupt_desc_table[238] = interrupt_gate_init((uint32_t)stub_238, selector, 0);
    interrupt_desc_table[239] = interrupt_gate_init((uint32_t)stub_239, selector, 0);
    interrupt_desc_table[240] = interrupt_gate_init((uint32_t)stub_240, selector, 0);
    interrupt_desc_table[241] = interrupt_gate_init((uint32_t)stub_241, selector, 0);
    interrupt_desc_table[242] = interrupt_gate_init((uint32_t)stub_242, selector, 0);
    interrupt_desc_table[243] = interrupt_gate_init((uint32_t)stub_243, selector, 0);
    interrupt_desc_table[244] = interrupt_gate_init((uint32_t)stub_244, selector, 0);
    interrupt_desc_table[245] = interrupt_gate_init((uint32_t)stub_245, selector, 0);
    interrupt_desc_table[246] = interrupt_gate_init((uint32_t)stub_246, selector, 0);
    interrupt_desc_table[247] = interrupt_gate_init((uint32_t)stub_247, selector, 0);
    interrupt_desc_table[248] = interrupt_gate_init((uint32_t)stub_248, selector, 0);
    interrupt_desc_table[249] = interrupt_gate_init((uint32_t)stub_249, selector, 0);
    interrupt_desc_table[250] = interrupt_gate_init((uint32_t)stub_250, selector, 0);
    interrupt_desc_table[251] = interrupt_gate_init((uint32_t)stub_251, selector, 0);
    interrupt_desc_table[252] = interrupt_gate_init((uint32_t)stub_252, selector, 0);
    interrupt_desc_table[253] = interrupt_gate_init((uint32_t)stub_253, selector, 0);
    interrupt_desc_table[254] = interrupt_gate_init((uint32_t)stub_254, selector, 0);
    interrupt_desc_table[255] = interrupt_gate_init((uint32_t)stub_255, selector, 0);


    for(int i = 0; i < 256; i++) interrupt_handlers[i] = 0;

    pseudo_descriptor_t idtp = (pseudo_descriptor_t){
        .base = (uint32_t) &interrupt_desc_table,
        .limit = sizeof(interrupt_desc_table)-1
    };

    idtr_write(idtp);

    pseudo_descriptor_t c  = idtr_read();

    assert(c.base == (uint32_t)&interrupt_desc_table, "idt base not set well");
    assert(c.limit == sizeof(interrupt_desc_table)-1, "idt limit not set well");
}

void interrupt_handler_register(uint32_t index, interrupt_routine handler) {
    assert(index < 256, "handler index out of bounds");
    interrupt_handlers[index] = handler;
}



void enable_interrupts() {
    asm volatile("sti");
}

void disable_interrupts() {
    asm volatile("cli");
}