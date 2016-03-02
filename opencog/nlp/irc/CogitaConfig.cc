/*
 *   Config class for La Cogita IRC chatbot
 *   Copyright (C) 2009 Joel Pitt <joel@fruitionnz.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "CogitaConfig.h"

#include <getopt.h>

#include <string>
#include <vector>
#include <iostream>
#include <stdlib.h>

#include <opencog/util/StringTokenizer.h>

using std::cout;
using std::string;

using namespace std;

namespace opencog {
namespace chatbot {

#define DEFAULT_SERVER "irc.freenode.net"
#define DEFAULT_PORT 6667
#define DEFAULT_NICK "cogita-bot"
#define DEFAULT_CHANNELS { "#opencog", 0 }
#define VERSION "1.0.1"
#define VSTRING "La Cogita OpenCog (http://opencog.org) IRC chatbot version "  VERSION
#define DEFAULT_ATTN { "cogita-bot", "cogita", "cog", 0 }
#define DEFAULT_ATTN_SUFFIXES { ",", ":", 0 }

#define DEFAULT_COG_IP "127.0.0.1"
#define DEFAULT_COG_PORT 17004


CogitaConfig::CogitaConfig() :
    version_string(VSTRING),
    ircNetwork(DEFAULT_SERVER),
    ircPort(DEFAULT_PORT),
    nick(DEFAULT_NICK),
    dry_run(false),
    cog_addr(DEFAULT_COG_IP),
    cog_port(DEFAULT_COG_PORT)
{
    const char* defaultAttns[] = DEFAULT_ATTN;
    const char* defaultSuffixes[] = DEFAULT_ATTN_SUFFIXES;
    const char* defaultChannels[] = DEFAULT_CHANNELS;
    for (int i = 0; defaultAttns[i]; i++) {
        for (int i = 0; defaultSuffixes[i]; i++) {
            attn.push_back(string(defaultAttns[i]) +
                    string(defaultSuffixes[i]));
        }
    }
    for (int i = 0; defaultChannels[i]; i++) {
        ircChannels.push_back(std::string(defaultChannels[i]));
    }
}

const char * CogitaConfig::helpOutput =
    " Cogita - An OpenCog IRC chatbot, version " VERSION "\n"
    " ======\n"
    " Usage: \n"
    " -n,--nick \tSet bot nick. (default: %s)\n"
    " -s,--server \tIRC server to connect to. (default: %s)\n"
    " -p,--port \tPort of IRC server to connect to. (default: %d)\n"
    " -c,--channel \tChannel (without #) to join (default: %s)\n"
    " -o,--cogserver \tCogserver to use (default: %s)\n"
    " -t,--cog-port \tCogserver port number (default: %d)\n"
    " -d,--dry-run \tPrint settings and quit.\n"
    " -v,--version \tPrint version information.\n"
    " \n";

void CogitaConfig::printHelp()
{
#define BUFSZ 8190
    char buff[BUFSZ];
    snprintf(buff, BUFSZ, helpOutput, nick.c_str(), ircNetwork.c_str(),
             ircPort, ircChannels[0].c_str());
    cout << buff;
}

void CogitaConfig::printVersion() { cout << VSTRING << endl; }

int CogitaConfig::parseOptions(int argc, char* argv[])
{
    int c = 0;
    static const char *optString =
        "n:s:p:c:o:t:dvh";

    static const struct option longOptions[] = {
        {"nick", required_argument, 0, 'n'},
        {"server", required_argument, 0, 's'},
        {"port", required_argument, 0, 's'},
        {"channel", required_argument, 0, 'c'},
        {"cogserver", required_argument, 0, 'o'},
        {"cog-port", required_argument, 0, 't'},
        {"dry-run", 0, 0, 'd'},
        {"version", 0, 0, 'v'},
        {"help", 0, 0, '?'},
        {0, 0, 0, 0}
    };

    while (1) {
        int optionIndex = 0;
        string channelsTemp;
        StringTokenizer st;
        c = getopt_long (argc, argv, optString, longOptions, &optionIndex);

        /* Detect end of options */
        if (c == -1) break;

        switch (c) {
        case 'n':
            nick = string(optarg);
            createAttnVector();
            break;
        case 's':
            ircNetwork = string(optarg);
            break;
        case 'o':
            cog_addr = string(optarg);
            break;
        case 'p':
            ircPort = atoi(optarg);
            break;
        case 't':
            cog_port = atoi(optarg);
            break;
        case 'c':
            ircChannels.clear();
            channelsTemp = optarg;
            st.set_string(channelsTemp);
            st.set_delimiter(string(","));
            for (string channel = st.next_token();
                    channel.size() > 0;
                    channel = st.next_token()) {
                ircChannels.push_back("#" + channel);
            }
            break;
        case 'd': dry_run = true; break;
        case 'v':
            printVersion();
            return 1;
        case 'h':
            printHelp();
            return 1;
        default:
            printHelp();
            return 1;
        }
    }

    return 0;
}

void CogitaConfig::createAttnVector()
{
    const char* defaultSuffixes[] = DEFAULT_ATTN_SUFFIXES;
    attn.clear();
    for (int i = 0; defaultSuffixes[i]; i++) {
        attn.push_back(nick + string(defaultSuffixes[i]));
    }
}

}} // ~namespace opencog::chatbot
