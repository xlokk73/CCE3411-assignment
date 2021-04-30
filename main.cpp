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
 #include "ns3/object-factory.h"

void debug(std::string str) {
    std::cout << str << std::endl;
}



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

    //PointToPointHelper pointToPointHelper;
    //pointToPointHelper.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    //pointToPointHelper.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer netDeviceContainer;
    //netDeviceContainer = pointToPointHelper.Install(nodeContainer);

    /* Added code to replace PointToPointHelper*/
    /**/
    PointToPointNetDevice pointToPointNetDeviceA;
    pointToPointNetDeviceA.SetDataRate(DataRate("5Mbps"));

    PointToPointNetDevice pointToPointNetDeviceB;
    pointToPointNetDeviceB.SetDataRate(DataRate("5Mbps"));

    PointToPointChannel pointToPointChannel;

    netDeviceContainer.Add(&pointToPointNetDeviceA);
    netDeviceContainer.Add(&pointToPointNetDeviceB);

    nodeContainer.Get(0)->AddDevice(netDeviceContainer.Get(0));
    nodeContainer.Get(1)->AddDevice(netDeviceContainer.Get(1));

    netDeviceContainer.Get(0)->SetNode(nodeContainer.Get(0));
    netDeviceContainer.Get(1)->SetNode(nodeContainer.Get(1));

    pointToPointChannel.Attach(&pointToPointNetDeviceA);
    pointToPointChannel.Attach(&pointToPointNetDeviceB);

    /**/
    /* End of added code for PointToPointHelper*/

    //InternetStackHelper internetStackHelper;
    //internetStackHelper.Install(nodeContainer);

    ObjectFactory arpL3Factory;
    arpL3Factory.SetTypeId("ns3::ArpL3Protocol");
    Ptr<Object> arpL3Protocol = arpL3Factory.Create <Object> ();
    nodeContainer.Get(0)->AggregateObject (arpL3Protocol);

    ObjectFactory ipv4L3Factory;
    ipv4L3Factory.SetTypeId("ns3::Ipv4L3Protocol");
    Ptr<Object> ipv4L3Protocol = ipv4L3Factory.Create <Object> ();
    nodeContainer.Get(0)->AggregateObject (ipv4L3Protocol);

    ObjectFactory icmpv4L4Factory;
    icmpv4L4Factory.SetTypeId("ns3::Icmpv4L4Protocol");
    Ptr<Object> icmpv4L4Protocol = icmpv4L4Factory.Create<Object> ();
    nodeContainer.Get(0)->AggregateObject(icmpv4L4Protocol);

    Ptr<Ipv4> ipv4 = nodeContainer.Get(0)->GetObject<Ipv4> ();
    Ptr<Ipv4RoutingProtocol> ipv4RoutingProtocol = CreateObject<Ipv4StaticRouting>();
    ipv4->SetRoutingProtocol (ipv4RoutingProtocol);

    ObjectFactory trafficControlLayerFactory;
    trafficControlLayerFactory.SetTypeId("ns3::TrafficControlLayer");
    Ptr<Object> trafficControlLayerProtocol = trafficControlLayerFactory.Create<Object> ();
    nodeContainer.Get(0)->AggregateObject(trafficControlLayerProtocol);

    ObjectFactory udpL4Factory;
    udpL4Factory.SetTypeId("ns3::UdpL4Protocol");
    Ptr<Object> udpL4Protocol = udpL4Factory.Create<Object> ();
    nodeContainer.Get(0)->AggregateObject(udpL4Protocol);

    Ptr<PacketSocketFactory> packetSocketFactory = CreateObject<PacketSocketFactory> ();
    nodeContainer.Get(0)->AggregateObject (packetSocketFactory);





    std::cout << "ADDED, OK!\n";

    /* End of added code */

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

    //pointToPointHelper.EnablePcapAll("firstPcap");
    Simulator::Run ();
    Simulator::Destroy ();

    return 0;
}
