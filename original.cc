
/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


 #include "ns3/core-module.h"
 #include "ns3/network-module.h"
 #include "ns3/internet-module.h"
 #include "ns3/point-to-point-module.h"
 #include "ns3/applications-module.h"


using namespace ns3;

int main(int argc, char* argv[])
{
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);
    ns3::LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    ns3::LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    NodeContainer nodeContainer;
    nodeContainer.Create(2);

    PointToPointHelper pointToPointHelper;
    pointToPointHelper.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPointHelper.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer netDeviceContainer;
    netDeviceContainer = pointToPointHelper.Install(nodeContainer);

    InternetStackHelper internetStackHelper;
    internetStackHelper.Install(nodeContainer);

    Ipv4AddressHelper ipv4AddressHelper;
    ipv4AddressHelper.SetBase("192.168.0.0", "255.255.255.0");

    Ipv4InterfaceContainer ipv4InterfaceContainer = ipv4AddressHelper.Assign(netDeviceContainer);

    UdpEchoServerHelper udpEchoServerHelper(9);
    ApplicationContainer serverApplicationContainer = udpEchoServerHelper.Install(nodeContainer.Get(1));
    serverApplicationContainer.Start(Seconds(1.0));
    serverApplicationContainer.Stop(Seconds(10.0));

    UdpEchoClientHelper udpEchoClientHelper(ipv4InterfaceContainer.GetAddress(1), 9);
    udpEchoClientHelper.SetAttribute("MaxPackets", UintegerValue(1));
    udpEchoClientHelper.SetAttribute("Interval", TimeValue(Seconds(2.0)));
    udpEchoClientHelper.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApplicationContainer = udpEchoClientHelper.Install(nodeContainer.Get(0));
    clientApplicationContainer.Start(Seconds(3.0));
    clientApplicationContainer.Stop(Seconds(10.0));

    pointToPointHelper.EnablePcapAll("firstPcap");
    Simulator::Run ();
    Simulator::Destroy ();

    return 0;
}
