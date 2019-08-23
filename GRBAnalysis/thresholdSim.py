import math
from ROOT import TH1F, TH2F, TFile, TTree, TCanvas,gROOT,TList,gStyle, TLatex, Double,TMemFile,gRandom, gDirectory,Long,gPad
import numpy as np

def calResolution(x):
    return math.sqrt(0.3*0.3+(5.56/(9*0.85*0.2*x)))

def getSpectrum():
    hspec=TH1F("hspec","hspec",300,0,300)
    hspec.SetBinContent(2,4753)
    hspec.SetBinContent(3,3859)
    hspec.SetBinContent(4,3250)
    hspec.SetBinContent(5,2780)
    hspec.SetBinContent(6,2523)
    hspec.SetBinContent(7,2226)
    hspec.SetBinContent(8,1991)
    hspec.SetBinContent(9,1850)
    hspec.SetBinContent(10,1761)
    hspec.SetBinContent(11,1638)
    hspec.SetBinContent(12,1516)
    hspec.SetBinContent(13,1546)
    hspec.SetBinContent(14,1459)
    hspec.SetBinContent(15,1459)
    hspec.SetBinContent(16,1418)
    hspec.SetBinContent(17,1330)
    hspec.SetBinContent(18,1284)
    hspec.SetBinContent(19,1259)
    hspec.SetBinContent(20,1226)
    hspec.SetBinContent(21,1157)
    hspec.SetBinContent(22,1107)
    hspec.SetBinContent(23,1071)
    hspec.SetBinContent(24,1028)
    hspec.SetBinContent(25,1062)
    hspec.SetBinContent(26,972)
    hspec.SetBinContent(27,950)
    hspec.SetBinContent(28,927)
    hspec.SetBinContent(29,833)
    hspec.SetBinContent(30,773)
    hspec.SetBinContent(31,803)
    hspec.SetBinContent(32,705)
    hspec.SetBinContent(33,769)
    hspec.SetBinContent(34,708)
    hspec.SetBinContent(35,697)
    hspec.SetBinContent(36,656)
    hspec.SetBinContent(37,637)
    hspec.SetBinContent(38,615)
    hspec.SetBinContent(39,605)
    hspec.SetBinContent(40,560)
    hspec.SetBinContent(41,567)
    hspec.SetBinContent(42,548)
    hspec.SetBinContent(43,516)
    hspec.SetBinContent(44,547)
    hspec.SetBinContent(45,539)
    hspec.SetBinContent(46,449)
    hspec.SetBinContent(47,455)
    hspec.SetBinContent(48,464)
    hspec.SetBinContent(49,449)
    hspec.SetBinContent(50,450)
    hspec.SetBinContent(51,420)
    hspec.SetBinContent(52,431)
    hspec.SetBinContent(53,372)
    hspec.SetBinContent(54,398)
    hspec.SetBinContent(55,411)
    hspec.SetBinContent(56,395)
    hspec.SetBinContent(57,370)
    hspec.SetBinContent(58,381)
    hspec.SetBinContent(59,351)
    hspec.SetBinContent(60,329)
    hspec.SetBinContent(61,340)
    hspec.SetBinContent(62,314)
    hspec.SetBinContent(63,329)
    hspec.SetBinContent(64,310)
    hspec.SetBinContent(65,332)
    hspec.SetBinContent(66,303)
    hspec.SetBinContent(67,326)
    hspec.SetBinContent(68,294)
    hspec.SetBinContent(69,312)
    hspec.SetBinContent(70,283)
    hspec.SetBinContent(71,272)
    hspec.SetBinContent(72,237)
    hspec.SetBinContent(73,272)
    hspec.SetBinContent(74,292)
    hspec.SetBinContent(75,252)
    hspec.SetBinContent(76,248)
    hspec.SetBinContent(77,289)
    hspec.SetBinContent(78,251)
    hspec.SetBinContent(79,235)
    hspec.SetBinContent(80,236)
    hspec.SetBinContent(81,246)
    hspec.SetBinContent(82,229)
    hspec.SetBinContent(83,214)
    hspec.SetBinContent(84,204)
    hspec.SetBinContent(85,246)
    hspec.SetBinContent(86,215)
    hspec.SetBinContent(87,184)
    hspec.SetBinContent(88,222)
    hspec.SetBinContent(89,198)
    hspec.SetBinContent(90,186)
    hspec.SetBinContent(91,223)
    hspec.SetBinContent(92,173)
    hspec.SetBinContent(93,204)
    hspec.SetBinContent(94,215)
    hspec.SetBinContent(95,192)
    hspec.SetBinContent(96,196)
    hspec.SetBinContent(97,199)
    hspec.SetBinContent(98,182)
    hspec.SetBinContent(99,174)
    hspec.SetBinContent(100,188)
    hspec.SetBinContent(101,163)
    hspec.SetBinContent(102,178)
    hspec.SetBinContent(103,193)
    hspec.SetBinContent(104,168)
    hspec.SetBinContent(105,154)
    hspec.SetBinContent(106,168)
    hspec.SetBinContent(107,161)
    hspec.SetBinContent(108,174)
    hspec.SetBinContent(109,153)
    hspec.SetBinContent(110,184)
    hspec.SetBinContent(111,176)
    hspec.SetBinContent(112,146)
    hspec.SetBinContent(113,146)
    hspec.SetBinContent(114,169)
    hspec.SetBinContent(115,139)
    hspec.SetBinContent(116,154)
    hspec.SetBinContent(117,150)
    hspec.SetBinContent(118,136)
    hspec.SetBinContent(119,143)
    hspec.SetBinContent(120,154)
    hspec.SetBinContent(121,144)
    hspec.SetBinContent(122,141)
    hspec.SetBinContent(123,146)
    hspec.SetBinContent(124,115)
    hspec.SetBinContent(125,136)
    hspec.SetBinContent(126,123)
    hspec.SetBinContent(127,145)
    hspec.SetBinContent(128,138)
    hspec.SetBinContent(129,125)
    hspec.SetBinContent(130,122)
    hspec.SetBinContent(131,116)
    hspec.SetBinContent(132,117)
    hspec.SetBinContent(133,124)
    hspec.SetBinContent(134,104)
    hspec.SetBinContent(135,116)
    hspec.SetBinContent(136,125)
    hspec.SetBinContent(137,112)
    hspec.SetBinContent(138,110)
    hspec.SetBinContent(139,102)
    hspec.SetBinContent(140,93)
    hspec.SetBinContent(141,89)
    hspec.SetBinContent(142,106)
    hspec.SetBinContent(143,112)
    hspec.SetBinContent(144,116)
    hspec.SetBinContent(145,121)
    hspec.SetBinContent(146,112)
    hspec.SetBinContent(147,104)
    hspec.SetBinContent(148,99)
    hspec.SetBinContent(149,124)
    hspec.SetBinContent(150,68)
    hspec.SetBinContent(151,86)
    hspec.SetBinContent(152,94)
    hspec.SetBinContent(153,108)
    hspec.SetBinContent(154,71)
    hspec.SetBinContent(155,91)
    hspec.SetBinContent(156,88)
    hspec.SetBinContent(157,92)
    hspec.SetBinContent(158,96)
    hspec.SetBinContent(159,90)
    hspec.SetBinContent(160,80)
    hspec.SetBinContent(161,108)
    hspec.SetBinContent(162,99)
    hspec.SetBinContent(163,86)
    hspec.SetBinContent(164,75)
    hspec.SetBinContent(165,86)
    hspec.SetBinContent(166,76)
    hspec.SetBinContent(167,78)
    hspec.SetBinContent(168,81)
    hspec.SetBinContent(169,90)
    hspec.SetBinContent(170,73)
    hspec.SetBinContent(171,79)
    hspec.SetBinContent(172,76)
    hspec.SetBinContent(173,79)
    hspec.SetBinContent(174,73)
    hspec.SetBinContent(175,82)
    hspec.SetBinContent(176,77)
    hspec.SetBinContent(177,70)
    hspec.SetBinContent(178,70)
    hspec.SetBinContent(179,56)
    hspec.SetBinContent(180,78)
    hspec.SetBinContent(181,75)
    hspec.SetBinContent(182,79)
    hspec.SetBinContent(183,60)
    hspec.SetBinContent(184,64)
    hspec.SetBinContent(185,69)
    hspec.SetBinContent(186,69)
    hspec.SetBinContent(187,77)
    hspec.SetBinContent(188,64)
    hspec.SetBinContent(189,64)
    hspec.SetBinContent(190,64)
    hspec.SetBinContent(191,72)
    hspec.SetBinContent(192,64)
    hspec.SetBinContent(193,69)
    hspec.SetBinContent(194,54)
    hspec.SetBinContent(195,67)
    hspec.SetBinContent(196,69)
    hspec.SetBinContent(197,60)
    hspec.SetBinContent(198,71)
    hspec.SetBinContent(199,59)
    hspec.SetBinContent(200,58)
    hspec.SetBinContent(201,61)
    hspec.SetBinContent(202,48)
    hspec.SetBinContent(203,45)
    hspec.SetBinContent(204,54)
    hspec.SetBinContent(205,57)
    hspec.SetBinContent(206,54)
    hspec.SetBinContent(207,62)
    hspec.SetBinContent(208,47)
    hspec.SetBinContent(209,50)
    hspec.SetBinContent(210,47)
    hspec.SetBinContent(211,59)
    hspec.SetBinContent(212,50)
    hspec.SetBinContent(213,54)
    hspec.SetBinContent(214,54)
    hspec.SetBinContent(215,55)
    hspec.SetBinContent(216,42)
    hspec.SetBinContent(217,45)
    hspec.SetBinContent(218,47)
    hspec.SetBinContent(219,48)
    hspec.SetBinContent(220,52)
    hspec.SetBinContent(221,43)
    hspec.SetBinContent(222,47)
    hspec.SetBinContent(223,50)
    hspec.SetBinContent(224,55)
    hspec.SetBinContent(225,49)
    hspec.SetBinContent(226,54)
    hspec.SetBinContent(227,42)
    hspec.SetBinContent(228,53)
    hspec.SetBinContent(229,46)
    hspec.SetBinContent(230,43)
    hspec.SetBinContent(231,47)
    hspec.SetBinContent(232,53)
    hspec.SetBinContent(233,35)
    hspec.SetBinContent(234,43)
    hspec.SetBinContent(235,39)
    hspec.SetBinContent(236,57)
    hspec.SetBinContent(237,43)
    hspec.SetBinContent(238,37)
    hspec.SetBinContent(239,55)
    hspec.SetBinContent(240,41)
    hspec.SetBinContent(241,41)
    hspec.SetBinContent(242,51)
    hspec.SetBinContent(243,41)
    hspec.SetBinContent(244,46)
    hspec.SetBinContent(245,38)
    hspec.SetBinContent(246,39)
    hspec.SetBinContent(247,51)
    hspec.SetBinContent(248,34)
    hspec.SetBinContent(249,44)
    hspec.SetBinContent(250,41)
    hspec.SetBinContent(251,44)
    hspec.SetBinContent(252,38)
    hspec.SetBinContent(253,41)
    hspec.SetBinContent(254,52)
    hspec.SetBinContent(255,32)
    hspec.SetBinContent(256,32)
    hspec.SetBinContent(257,43)
    hspec.SetBinContent(258,28)
    hspec.SetBinContent(259,40)
    hspec.SetBinContent(260,30)
    hspec.SetBinContent(261,46)
    hspec.SetBinContent(262,38)
    hspec.SetBinContent(263,44)
    hspec.SetBinContent(264,31)
    hspec.SetBinContent(265,26)
    hspec.SetBinContent(266,37)
    hspec.SetBinContent(267,28)
    hspec.SetBinContent(268,31)
    hspec.SetBinContent(269,43)
    hspec.SetBinContent(270,31)
    hspec.SetBinContent(271,39)
    hspec.SetBinContent(272,36)
    hspec.SetBinContent(273,35)
    hspec.SetBinContent(274,27)
    hspec.SetBinContent(275,24)
    hspec.SetBinContent(276,34)
    hspec.SetBinContent(277,27)
    hspec.SetBinContent(278,28)
    hspec.SetBinContent(279,40)
    hspec.SetBinContent(280,29)
    hspec.SetBinContent(281,29)
    hspec.SetBinContent(282,38)
    hspec.SetBinContent(283,25)
    hspec.SetBinContent(284,29)
    hspec.SetBinContent(285,45)
    hspec.SetBinContent(286,25)
    hspec.SetBinContent(287,28)
    hspec.SetBinContent(288,28)
    hspec.SetBinContent(289,33)
    hspec.SetBinContent(290,33)
    hspec.SetBinContent(291,35)
    hspec.SetBinContent(292,30)
    hspec.SetBinContent(293,19)
    hspec.SetBinContent(294,27)
    hspec.SetBinContent(295,30)
    hspec.SetBinContent(296,23)
    hspec.SetBinContent(297,22)
    hspec.SetBinContent(298,21)
    hspec.SetBinContent(299,37)
    hspec.SetBinContent(300,31)
    return hspec



def sim(thr,hspec):

    hsim=TH1F("hsim%d"%thr,"simulation; Energy (keV); Counts",300,0,100)
    for i in np.arange(0,1e5,1):
        rndE=hspec.GetRandom()
        rndthr=gRandom.Gaus(thr, 5)
        #calResolution(rndE)
        if rndE>rndthr:
            hsim.Fill(rndE)
    return hsim


c1=TCanvas()
#c1.Divide(2,2)
hspec=getSpectrum()
hsim1=sim(1,hspec)
hsim1.SetLineColor(1)

#c1.cd(1)
#hspec.SetLineColor(1)
#hspec.Draw()
hsim1.SetTitle('No threshold cut')
hsim1.Draw()
#c1.cd(2)
hsim10=sim(10,hspec)
hsim10.SetTitle("thr 10 keV ")
hsim10.SetLineColor(2)
hsim10.Draw("same")
#c1.cd(3)
hsim15=sim(20,hspec)
hsim15.SetTitle("thr 20 keV ")
hsim15.SetLineColor(3)
hsim15.Draw("same")
#c1.cd(4)
hsim20=sim(30,hspec)
hsim20.SetTitle("thr 30 keV ")
hsim20.SetLineColor(4)
hsim20.Draw("same")
gPad.BuildLegend()




    


