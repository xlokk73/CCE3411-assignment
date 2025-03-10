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
 #include "ns3/traffic-control-layer.h"
 #include "ns3/traffic-control-helper.h"
 #include "ns3/trace-helper.h"  
 #include "ns3/object-base.h"

#include "ns3/assert.h"
#include "ns3/net-device-container.h"
#include "ns3/node-container.h"
#include "ns3/simulator.h"
#include "ns3/pcap-file-wrapper.h"
#include "ns3/output-stream-wrapper.h"
#include "ns3/ptr.h"
 
using namespace ns3;


void debug(std::string str) {
    std::cout << str << std::endl;
}

void
DefaultSink (Ptr<PcapFileWrapper> file, Ptr<const Packet> p) {
  file->Write (Simulator::Now (), p);
}


int main(int argc, char* argv[])
{
    
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);
    ns3::LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    ns3::LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    //NodeContainer nodeContainer;
    //nodeContainer.Create(2);

    //PointToPointHelper pointToPointHelper;
    //pointToPointHelper.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    //pointToPointHelper.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer netDeviceContainer;
    //netDeviceContainer = pointToPointHelper.Install(nodeContainer);

    /* Added code to replace PointToPointHelper*/
    /**/
    // create channel
    Ptr<PointToPointChannel> pointToPointChannel = CreateObjectWithAttributes<PointToPointChannel>("Delay", StringValue("3ms"));


    // create a node
    Ptr<Node> nodeA = CreateObject<Node>();

    // create net device
    Ptr<PointToPointNetDevice> pointToPointNetDeviceA = CreateObjectWithAttributes<PointToPointNetDevice>("DataRate", StringValue("5Mbps"));
    nodeA->AddDevice(pointToPointNetDeviceA);

    // create and set queue to net device
    Ptr<Queue<Packet> > queueA = CreateObject<DropTailQueue<Packet> > ();
    pointToPointNetDeviceA->SetQueue(queueA);

    // attach channel to device
    pointToPointNetDeviceA->Attach(pointToPointChannel);

    netDeviceContainer.Add(pointToPointNetDeviceA);


    // create node
    Ptr<Node> nodeB = CreateObject<Node>();

    // create net device
    Ptr<PointToPointNetDevice> pointToPointNetDeviceB = CreateObjectWithAttributes<PointToPointNetDevice>("DataRate", StringValue("5Mbps"));
    nodeB->AddDevice(pointToPointNetDeviceB);

    // create and set queue to net deivce
    Ptr<Queue<Packet> > queueB = CreateObject<DropTailQueue<Packet> > ();
    pointToPointNetDeviceB->SetQueue(queueB);

    // attach channel to net device
    pointToPointNetDeviceB->Attach(pointToPointChannel);

    netDeviceContainer.Add(pointToPointNetDeviceB);

    /* End of added code for PointToPointHelper*/

    //InternetStackHelper internetStackHelper;
    //internetStackHelper.Install(nodeContainer);

    /* Added code to replace InternetStackHelper */

    // set nodes protocols
    Ptr<ArpL3Protocol> arpL3ProtocolA = CreateObject<ArpL3Protocol>();
    nodeA->AggregateObject(arpL3ProtocolA);

    Ptr<Ipv4L3Protocol> ipv4L3ProtocolA = CreateObject<Ipv4L3Protocol>();
    nodeA->AggregateObject(ipv4L3ProtocolA);

    Ptr<Icmpv4L4Protocol> icmpv4L4ProtocolA = CreateObject<Icmpv4L4Protocol>();
    nodeA->AggregateObject(icmpv4L4ProtocolA);

    Ptr<Ipv4> ipv4A = nodeA->GetObject<Ipv4> ();
    Ptr<Ipv4StaticRouting> ipv4StaticRoutingA = CreateObject<Ipv4StaticRouting>();
    ipv4A->SetRoutingProtocol(ipv4StaticRoutingA);

    Ptr<TrafficControlLayer> trafficControlLayerA = CreateObject<TrafficControlLayer>();
    arpL3ProtocolA->SetTrafficControl(trafficControlLayerA);
    nodeA->AggregateObject(trafficControlLayerA);

    Ptr<UdpL4Protocol> udpL4ProtocolA = CreateObject<UdpL4Protocol>();
    nodeA->AggregateObject(udpL4ProtocolA);

    Ptr<PacketSocketFactory> packetSocketFactoryA = CreateObject<PacketSocketFactory> ();
    nodeA->AggregateObject(packetSocketFactoryA);


    Ptr<ArpL3Protocol> arpL3ProtocolB = CreateObject<ArpL3Protocol>();
    nodeB->AggregateObject(arpL3ProtocolB);

    Ptr<Ipv4L3Protocol> ipv4L3ProtocolB = CreateObject<Ipv4L3Protocol>();
    nodeB->AggregateObject(ipv4L3ProtocolB);

    Ptr<Icmpv4L4Protocol> icmpv4L4ProtocolB = CreateObject<Icmpv4L4Protocol>();
    nodeB->AggregateObject(icmpv4L4ProtocolB);

    Ptr<Ipv4> ipv4B = nodeB->GetObject<Ipv4> ();
    Ptr<Ipv4StaticRouting> ipv4StaticRoutingB = CreateObject<Ipv4StaticRouting>();
    ipv4B->SetRoutingProtocol(ipv4StaticRoutingB);

    Ptr<TrafficControlLayer> trafficControlLayerB = CreateObject<TrafficControlLayer>();
    arpL3ProtocolB->SetTrafficControl(trafficControlLayerB);
    nodeB->AggregateObject(trafficControlLayerB);

    Ptr<UdpL4Protocol> udpL4ProtocolB = CreateObject<UdpL4Protocol>();
    nodeB->AggregateObject(udpL4ProtocolB);

    Ptr<PacketSocketFactory> packetSocketFactoryB = CreateObject<PacketSocketFactory> ();
    nodeB->AggregateObject(packetSocketFactoryB);

    /* End of added code for PointToPointHelper*/


    //Ipv4AddressHelper ipv4AddressHelper;
    //ipv4AddressHelper.SetBase("192.168.0.0", "255.255.255.0");

    //Ipv4InterfaceContainer ipv4InterfaceContainer = ipv4AddressHelper.Assign(netDeviceContainer);

    /* Added code to replace Ipv4AddressHelper */
    /**/
    uint32_t network = 3232235520;
    uint32_t mask = 4294967040;
    uint32_t address = 3232235521;
    //uint32_t base = 0xffffffff;
    uint32_t shift = 0xffffffff;
    //uint32_t maxx = 0xffffffff;

    Ipv4InterfaceContainer ipv4InterfaceContainer;
    Ipv4Address ipv4Address;

    // set addresses
    for (uint32_t i = 0; i < netDeviceContainer.GetN(); ++i) {
        Ptr<NetDevice> device = netDeviceContainer.Get (i);
        Ptr<Node> node = device->GetNode ();

        Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();

        int32_t interface = ipv4->GetInterfaceForDevice (device);
        if (interface == -1) {
            interface = ipv4->AddInterface (device);
        }

        //~~
        Ipv4Address addr ((network << shift) | address);
        ipv4Address = addr;
        ++address;
        //Ipv4AddressGenerator::AddAllocated (addr);
        //~~
        Ipv4InterfaceAddress ipv4Addr = Ipv4InterfaceAddress (addr, mask);
        ipv4->AddAddress (interface, ipv4Addr);
        ipv4->SetMetric (interface, 1);
        ipv4->SetUp (interface);
        ipv4InterfaceContainer.Add (ipv4, interface);
        Ptr<TrafficControlLayer> tc = node->GetObject<TrafficControlLayer> ();

        if (tc && DynamicCast<LoopbackNetDevice> (device) == 0 && tc->GetRootQueueDiscOnDevice (device) == 0) {
            Ptr<NetDeviceQueueInterface> ndqi = device->GetObject<NetDeviceQueueInterface> ();

            if (ndqi) {
                std::size_t nTxQueues = ndqi->GetNTxQueues ();

                TrafficControlHelper tcHelper = TrafficControlHelper::Default (nTxQueues);
                tcHelper.Install (device);
            }
        }
    }

    /**/
    /* End of added code for Ipv4AddressHelper */

    /**
    UdpEchoServerHelper udpEchoServerHelper(9);
    ApplicationContainer serverApplicationContainer = udpEchoServerHelper.Install(nodeContainer.Get(1));
    serverApplicationContainer.Start(Seconds(1.0));
    serverApplicationContainer.Stop(Seconds(10.0));
    **/

    /* Added code to replace UdpEchoServerHelper */
    /**/
    // create server application
    Ptr<UdpEchoServer> udpEchoServer = CreateObjectWithAttributes<UdpEchoServer> ("Port", UintegerValue (9));
    // set object as application for node
    nodeB->AddApplication(udpEchoServer);
    udpEchoServer->SetStartTime(Seconds(1.0));
    udpEchoServer->SetStopTime(Seconds(20.0));

    /**/
    /* End of added code for Ipv4AddressHelper */

    /**
    UdpEchoClientHelper udpEchoClientHelper(ipv4InterfaceContainer.GetAddress(1), 9);
    udpEchoClientHelper.SetAttribute("MaxPackets", UintegerValue(1));
    udpEchoClientHelper.SetAttribute("Interval", TimeValue(Seconds(2.0)));
    udpEchoClientHelper.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApplicationContainer = udpEchoClientHelper.Install(nodeContainer.Get(0));
    clientApplicationContainer.Start(Seconds(3.0));
    clientApplicationContainer.Stop(Seconds(10.0));
    **/

    /* Added code to replace UdpEchoClientHelper */
    /**/
    // create client application
    Ptr<UdpEchoClient> udpEchoClient = CreateObjectWithAttributes<UdpEchoClient> ("RemoteAddress", AddressValue(ipv4InterfaceContainer.GetAddress(1)));
    // set attributes
    udpEchoClient->SetAttribute("RemotePort", UintegerValue (9));
    udpEchoClient->SetAttribute("MaxPackets", UintegerValue(1));
    udpEchoClient->SetAttribute("Interval", TimeValue(Seconds(2.0)));
    udpEchoClient->SetAttribute("PacketSize", UintegerValue(2048));

    // set object as application for node
    nodeA->AddApplication(udpEchoClient);
    udpEchoClient->SetStartTime(Seconds(3));
    udpEchoClient->SetStopTime(Seconds(10));

    /**/
    /* End of added code for Ipv4AddressHelper */


    /*
    PointToPointHelper pointToPointHelper;
    pointToPointHelper.EnablePcapAll("firstPcap");
    */

    /* Added code to replace pcap tracing helper code */
    // open file to write to
    Ptr<PcapFileWrapper> fileA = CreateObject<PcapFileWrapper>();
    fileA->Open("firstPcapA.pcap", std::ios::out);
    fileA->Init(9);

    std::string tracename = "PromiscSniffer";
    // use callback
    pointToPointNetDeviceA->TraceConnectWithoutContext(tracename.c_str(),  MakeBoundCallback (&DefaultSink, fileA));


    Ptr<PcapFileWrapper> fileB = CreateObject<PcapFileWrapper>();
    fileB->Open("firstPcapB.pcap", std::ios::out);
    fileB->Init(9);

    pointToPointNetDeviceB->TraceConnectWithoutContext(tracename.c_str(),  MakeBoundCallback (&DefaultSink, fileB));

    /* End of added code to replace pcap tracing helper code */

    Simulator::Run ();
    Simulator::Destroy ();


    return 0;
}

