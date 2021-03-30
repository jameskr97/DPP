#include <dpp/discord.h>
#include <dpp/event.h>
#include <string>
#include <iostream>
#include <fstream>
#include <dpp/discordclient.h>
#include <dpp/discord.h>
#include <dpp/cache.h>
#include <dpp/stringops.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void guild_create::handle(class DiscordClient* client, json &j) {
	dpp::guild* g = new dpp::guild();
	json& d = j["d"];
	g->fill_from_json(&d);
	if (!g->is_unavailable()) {
		/* Store guild roles */
		for (auto & role : d["roles"]) {
			dpp::role *r = new dpp::role();
			r->fill_from_json(&role);
			dpp::get_role_cache()->store(r);
			g->roles.push_back(r->id);
		}

		/* Store guild channels */
		for (auto & channel : d["channels"]) {
			dpp::channel *c = new dpp::channel();
			c->fill_from_json(&channel);
			dpp::get_channel_cache()->store(c);
			g->channels.push_back(c->id);
		}

		/* Store guild members */
		for (auto & user : d["members"]) {
			dpp::user *u = new dpp::user();
			u->fill_from_json(&(user["user"]));
			dpp::guild_member* gm = new dpp::guild_member();
			gm->fill_from_json(&user, g, u);

			g->members[u->id] = gm;
			dpp::get_user_cache()->store(u);
		}
	}
	dpp::get_guild_cache()->store(g);
}
