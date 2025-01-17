/************************************************************************************
 *
 * D++, A Lightweight C++ library for Discord
 *
 * Copyright 2021 Craig Edwards and D++ contributors
 * (https://github.com/brainboxdotcc/DPP/graphs/contributors)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ************************************************************************************/
#pragma once

#include <dpp/discord.h>
#include <optional>
#include <dpp/json_fwd.hpp>

namespace dpp {

/**
 * @brief Represents the type of a component
 */
enum component_type : uint8_t {
	/// Action row, a container for other components
	cot_action_row = 1,
	/// Clickable button
	cot_button
};

/**
 * @brief Represents the style of a button
 */
enum component_style : uint8_t {
	/// Blurple
	cos_primary = 1,
	/// Grey
	cos_secondary,
	/// Green
	cos_success,
	/// Red
	cos_danger,
	/// An external hyperlink to a website
	cos_link
};

/**
 * @brief Represents the component object.
 * A component is a clickable button or drop down list
 * within a discord message, where the buttons emit
 * on_button_click events when the user interacts with
 * them.
 *
 * You should generally define one component object and
 * then insert one or more additional components into it
 * using component::add_component(), so that the parent
 * object is an action row and the child objects are buttons.
 *
 * @note At present this only works for whitelisted
 * guilds. The beta is **closed**. When this feature is
 * released, then the functionality will work correctly.
 */
class component {
public:
	/** Component type, either a button or action row
	 */
	component_type type;

	/** Sub commponents, buttons on an action row
	 */
	std::vector<component> components;

	/** Component label (for buttons).
	 * Maximum of 80 characters.
	 */
	std::string label;

	/** Component style (for buttons)
	 */
	component_style style;

	/** Component id (for buttons).
	 * Maximum of 100 characters.
	 */
	std::string custom_id;

	/** URL for link types (dpp::cos_link).
	 * Maximum of 512 characters.
	 */
	std::string url;

	/** Disabled flag (for buttons)
	 */
	bool disabled;

	/** Emoji definition. To set an emoji on your button
	 * you must set one of either the name or id fields.
	 * The easiest way is to use the component::set_emoji
	 * method.
	 */
	struct inner_emoji {
		/** Set the name field to the name of the emoji.
		 * For built in unicode emojis, set this to the
		 * actual unicode value of the emoji e.g. "😄"
		 * and not for example ":smile:"
		 */
		std::string name;
		/** The emoji ID value for emojis that are custom
		 * ones belonging to a guild. The same rules apply
		 * as with other emojis, that the bot must be on
		 * the guild where the emoji resides and it must
		 * be available for use (e.g. not disabled due to
		 * lack of boosts etc)
		 */
		dpp::snowflake id;
		/** True if the emoji is animated. Only applies to
		 * custom emojis.
		 */
		bool animated;
	} emoji;

	/** Constructor
	 */
	component();

	/** Destructor
	 */
	~component() = default;

	/**
	 * @brief Set the type of the component. Button components
	 * (type dpp::cot_button) should always be contained within
	 * an action row (type dpp::cot_action_row). As described
	 * below, many of the other methods automatically set this
	 * to the correct type so usually you should not need to
	 * manually  call component::set_type().
	 *
	 * @param ct The component type
	 * @return component& reference to self
	 */
	component& set_type(component_type ct);

	/**
	 * @brief Set the label of the component, e.g. button text.
	 * For action rows, this field is ignored. Setting the
	 * label will auto-set the type to dpp::cot_button.
	 *
	 * @param label Label text to set
	 * @return component& Reference to self
	 */
	component& set_label(const std::string &label);

	/**
	 * @brief Set the url for dpp::cos_link types.
	 * Calling this function sets the style to dpp::cos_link
	 * and the type to dpp::cot_button.
	 *
	 * @param url URL to set, maximum length of 512 characters
	 * @return component& reference to self.
	 */
	component& set_url(const std::string &url);

	/**
	 * @brief Set the style of the component, e.g. button colour.
	 * For action rows, this field is ignored. Setting the
	 * style will auto-set the type to dpp::cot_button.
	 *
	 * @param cs Component style to set
	 * @return component& reference to self
	 */
	component& set_style(component_style cs);

	/**
	 * @brief Set the id of the component.
	 * For action rows, this field is ignored. Setting the
	 * id will auto-set the type to dpp::cot_button.
	 *
	 * @param id Custom ID string to set. This ID will be sent
	 * for any on_button_click events related to the button.
	 * @return component& Reference to self
	 */
	component& set_id(const std::string &id);

	/**
	 * @brief Set the component to disabled.
	 * Defaults to false on all created components.
	 *
	 * @param disable True to disable, false to disable.
	 * @return component&
	 */
	component& set_disabled(bool disable);

	/**
	 * @brief Add a sub-component, only valid for action rows.
	 * Adding subcomponents to a component will automatically
	 * set this component's type to dpp::cot_action_row.
	 *
	 * @param c The sub-component to add
	 * @return component& reference to self
	 */
	component& add_component(const component& c);

	/**
	 * @brief Set the emoji of the current sub-component.
	 * Only valid for buttons. Adding an emoji to a component
	 * will automatically set this components type to
	 * dpp::cot_button. One or both of name and id must be set.
	 * For a built in unicode emoji, you only need set name,
	 * and should set it to a unicode character e.g. "😄".
	 * For custom emojis, set the name to the name of the emoji
	 * on the guild, and the id to the emoji's ID.
	 * Setting the animated boolean is only valid for custom
	 * emojis.
	 *
	 * @param name Emoji name, or unicode character to use
	 * @param id Emoji id, for custom emojis only.
	 * @param animated True if the custom emoji is animated.
	 * @return component& Reference to self
	 */
	component& set_emoji(const std::string& name, dpp::snowflake id = 0, bool animated = false);

	/** Read class values from json object
	 * @param j A json object to read from
	 * @return A reference to self
	 */
	component& fill_from_json(nlohmann::json* j);

	/** Build JSON from this object.
	 * @return The JSON text of the invite
	 */
	std::string build_json() const;

};

/**
 * @brief A footer in a dpp::embed
 */
struct embed_footer {
	/** Footer text */
	std::string text;
	/** Footer icon url */
	std::string icon_url;
	/** Proxied icon url */
	std::string proxy_url;
};

/**
 * @brief An video, image or thumbnail in a dpp::embed
 */
struct embed_image {
	/** URL to image or video */
	std::string url;
	/** Proxied image url */
	std::string proxy_url;
	/** Height (calculated by discord) */
	std::string height;
	/** Width (calculated by discord) */
	std::string width;
};

/**
 * @brief Embed provider in a dpp::embed. Received from discord but cannot be sent
 */
struct embed_provider {
	/** Provider name */
	std::string name;
	/** Provider URL */
	std::string url;
};

/**
 * @brief Author within a dpp::embed object
 */
struct embed_author {
	/** Author name */
	std::string name;
	/** Author url */
	std::string url;
	/** Author icon url */
	std::string icon_url;
	/** Proxied icon url */
	std::string proxy_icon_url;
};

/**
 * @brief A dpp::embed may contain zero or more fields
 */
struct embed_field {
	/** Name of field */
	std::string name;
	/** Value of field (max length 1000) */
	std::string value;
	/** True if the field is to be displayed inline */
	bool is_inline;
};

/**
 * @brief A rich embed for display within a dpp::message
 */
struct embed {
	/** Optional: title of embed */
	std::string			title;
	/** Optional: type of embed (always "rich" for webhook embeds) */
	std::string			type;
	/** Optional: description of embed */
	std::string			description;
	/** Optional: url of embed */
	std::string			url;
	/** Optional: timestamp of embed content */
	time_t				timestamp;
	/** Optional: color code of the embed */
	uint32_t			color;
	/** Optional: footer information */
	std::optional<embed_footer>	footer;
	/** Optional: image information */
	std::optional<embed_image>	image;
	/** Optional: thumbnail information */
	std::optional<embed_image>	thumbnail;
	/** Optional: video information (can't send these) */
	std::optional<embed_image>	video;
	/** Optional: provider information (can't send these) */
	std::optional<embed_provider>	provider;
	/** Optional: author information */
	std::optional<embed_author>	author;
	/** Optional: fields information */
	std::vector<embed_field>	fields;

	/** Constructor */
	embed();

	/** Constructor to build embed from json object
	 * @param j JSON to read content from
	 */
	embed(nlohmann::json* j);

	/** Destructor */
	~embed();

	/** Set embed title. Returns the embed itself so these method calls may be "chained"
	 * @param text The text of the title
	 * @return A reference to self
	 */
	embed& set_title(const std::string &text);

	/** Set embed description. Returns the embed itself so these method calls may be "chained"
	 * @param text The text of the title
	 * @return A reference to self
	 */
	embed& set_description(const std::string &text);

	/** Set embed colour. Returns the embed itself so these method calls may be "chained"
	 * @param col The colour of the embed
	 * @return A reference to self
	 */
	embed& set_color(uint32_t col);

	/** Set embed url. Returns the embed itself so these method calls may be "chained"
	 * @param url the url of the embed
	 * @return A reference to self
	 */
	embed& set_url(const std::string &url);

	/** Add an embed field. Returns the embed itself so these method calls may be "chained"
	 * @param name The name of the field
	 * @param value The value of the field (max length 1000)
	 * @param is_inline Wether or not to display the field 'inline' or on its own line
	 * @return A reference to self
	 */
	embed& add_field(const std::string& name, const std::string &value, bool is_inline = false);

	/** Set embed author. Returns the embed itself so these method calls may be "chained"
	 * @param name The name of the author
	 * @param url The url of the author
	 * @param icon_url The icon URL of the author
	 * @return A reference to self
	 */

	embed& set_author(const std::string& name, const std::string& url, const std::string& icon_url);

	/** Set embed provider. Returns the embed itself so these method calls may be "chained"
	 * @param name The provider name
	 * @param url The provider url
	 * @return A reference to self
	 */
	embed& set_provider(const std::string& name, const std::string& url);

	/** Set embed image. Returns the embed itself so these method calls may be "chained"
	 * @param url The embed image URL
	 * @return A reference to self
	 */
	embed& set_image(const std::string& url);

	/** Set embed video. Returns the embed itself so these method calls may be "chained"
	 * @param url The embed video url
	 * @return A reference to self
	 */
	embed& set_video(const std::string& url);

	/** Set embed thumbnail. Returns the embed itself so these method calls may be "chained"
	 * @param url The embed thumbnail url
	 * @return A reference to self
	 */
	embed& set_thumbnail(const std::string& url);
};

/**
 * @brief Represets a reaction to a dpp::message
 */
struct reaction {
	/** Number of times this reaction has been added */
	uint32_t count;
	/** Reaction was from the bot's id */
	bool me;
	/** ID of emoji for reaction */
	snowflake emoji_id;
	/** Name of emoji, if applicable */
	std::string emoji_name;

	/**
	 * @brief Constructs a new reaction object.
	 */
	reaction();

	/**
	 * @brief Constructs a new reaction object from a JSON object.
	 * @param j The JSON to read data from
	 */
	reaction(nlohmann::json* j);

	/**
	 * @brief Destructs the reaction object.
	 */
	~reaction() = default;
};

/**
 * @brief Represents an attachment in a dpp::message
 */
struct attachment {
	/** ID of attachment */
	snowflake id;
	/** Size of the attachment in bytes */
	uint32_t size;
	/** File name of the attachment */
	std::string filename;
	/** URL which points to the attachment */
	std::string url;
	/** Proxied URL which points to the attachment */
	std::string proxy_url;
	/** Width of the attachment, if applicable */
	uint32_t width;
	/** Height of the attachment, if applicable */
	uint32_t height;
	/** MIME type of the attachment, if applicable */
	std::string content_type;

	/**
	 * @brief Constructs a new attachment object.
	 */
	attachment();

	/**
	 * @brief Constructs a new attachment object from a JSON object.
	 * @param j JSON to read information from
	 */
	attachment(nlohmann::json* j);

	/**
	 * @brief Destructs the attachment object.
	 */
	~attachment() = default;
};

/**
 * @brief Bitmask flags for a dpp::message
 */
enum message_flags {
	/// this message has been published to subscribed channels (via Channel Following)
	m_crossposted = 1 << 0,
	/// this message originated from a message in another channel (via Channel Following)
	m_is_crosspost =  1 << 1,
	/// do not include any embeds when serializing this message
	m_supress_embeds = 1 << 2,
	/// the source message for this crosspost has been deleted (via Channel Following)
	m_source_message_deleted = 1 << 3,
	/// this message came from the urgent message system
	m_urgent = 1 << 4,
	/// this message is only visible to the user who invoked the Interaction
	m_ephemeral = 1 << 6,
	/// this message is an Interaction Response and the bot is "thinking"
	m_loading = 1 << 7
};

/**
 * @brief Mesage types for dpp::message::type
 */
enum message_type {
	/// Default
	mt_default					= 0,
	/// Add recipient
	mt_recipient_add				= 1,
	/// Remove recipient
	mt_recipient_remove				= 2,
	/// Call
	mt_call						= 3,
	/// Channel name change
	mt_channel_name_change				= 4,
	/// Channel icon change
	mt_channel_icon_change				= 5,
	/// Message pinned
	mt_channel_pinned_message			= 6,
	/// Member joined
	mt_guild_member_join				= 7,
	/// Boost
	mt_user_premium_guild_subscription		= 8,
	/// Boost level 1
	mt_user_premium_guild_subscription_tier_1	= 9,
	/// Boost level 2
	mt_user_premium_guild_subscription_tier_2	= 10,
	/// Boost level 3
	mt_user_premium_guild_subscription_tier_3	= 11,
	/// Follow channel
	mt_channel_follow_add				= 12,
	/// Disqualified from discovery
	mt_guild_discovery_disqualified			= 14,
	/// Re-qualified for discovery
	mt_guild_discovery_requalified			= 15,
	/// Discovery grace period warning 1
	mt_guild_discovery_grace_period_initial_warning	= 16,
	/// Discovery grace period warning 2
	mt_guild_discovery_grace_period_final_warning	= 17,
	/// Reply
	mt_reply					= 19,
	/// Application command
	mt_application_command				= 20,
	/// Invite reminder
	mt_guild_invite_reminder			= 22
};

/**
 * @brief Represents the user caching policy of the cluster.
 */
enum cache_policy_t : uint8_t {
	/**
	 * @brief request whole member lists on seeing new guilds, and also store users when they message us
	 */
	cp_aggressive = 0,
	/**
	 * @brief only cache users/members when they message the bot
	 */
	cp_lazy = 1,
	/**
	 * @brief Don't cache anything. Fill member details when we see them.
	 * (NOT IMPLEMENTED YET)
	 */
	cp_none = 2
};

/**
 * @brief Represents messages sent and received on Discord
 */
struct message {
	/** id of the message */
	snowflake       id;
	/** id of the channel the message was sent in */
	snowflake       channel_id;
	/** Optional: id of the guild the message was sent in */
	snowflake       guild_id;
	/** the author of this message (not guaranteed to be a valid user) */
	user*		author;
	/** Optional: member properties for this message's author */
	guild_member	member;
	/** contents of the message */
	std::string	content;
	/** message components */
	std::vector<dpp::component> components;
	/** when this message was sent */
	time_t		sent;
	/** when this message was edited (may be 0 if never edited) */
	time_t		edited;
	/** whether this was a TTS message */
	bool		tts;
	/** whether this message mentions everyone */
	bool   		mention_everyone;
	/** users specifically mentioned in the message */
	std::vector<snowflake>	mentions;
	/** roles specifically mentioned in this message */
	std::vector<snowflake> mention_roles;
	/** Optional: channels specifically mentioned in this message */
	std::vector<snowflake> mention_channels;
	/** any attached files */
	std::vector<attachment> attachments;
	/** zero or more dpp::embed objects */
	std::vector<embed> embeds;
	/** Optional: reactions to the message */
	std::vector<reaction> reactions;
	/** Optional: used for validating a message was sent */
	std::string	nonce;
	/** whether this message is pinned */
	bool		pinned;
	/** Optional: if the message is generated by a webhook, its id will be here otherwise the field will be 0 */
	snowflake	webhook_id;
	/** Flags */
	uint8_t		flags;

	/** Name of file to upload (for use server-side in discord's url) */
	std::string	filename;

	/** File content to upload (raw binary) */
	std::string	filecontent;

	/** Message type */
	uint8_t		type;

	/** True if the message object allocated its own author user */
	bool		self_allocated;

	struct message_ref {
		snowflake message_id;		// id of the originating message
		snowflake channel_id;		// id of the originating message's channel
		snowflake guild_id;		// id of the originating message's guild
		bool fail_if_not_exists;	// when sending, whether to error if the referenced message doesn't exist instead of sending as a normal (non-reply) message, default true
	} message_reference;

	/**
	 * @brief Construct a new message object
	 */
	message();

	~message();

    /**
	 * @brief Construct a new message object with a channel and content
	 *
	 * @param channel_id The channel to send the message to
	 * @param content The content of the message
	 * @param type The message type to create
	 */
	message(snowflake channel_id, const std::string &content, message_type type = mt_default);

	/**
	 * @brief Construct a new message object with a channel and content
	 *
	 * @param channel_id The channel to send the message to
	 * @param _embed An embed to send
	 */
	message(snowflake channel_id, const embed & _embed);

	/**
	 * @brief Construct a new message object with content
	 *
	 * @param content The content of the message
	 * @param type The message type to create
	 */
	message(const std::string &content, message_type type = mt_default);

	/**
	 * @brief Set the original message reference for replies/crossposts
	 * 
	 * @param _message_id message id to reply to
	 * @param _guild_id guild id to reply to (optional)
	 * @param _channel_id channel id to reply to (optional)
	 * @param fail_if_not_exists true if the message send should fail if these values are invalid (optional)
	 * @return message& 
	 */
	message& set_reference(snowflake _message_id, snowflake _guild_id = 0, snowflake _channel_id = 0, bool fail_if_not_exists = false);

	/** Fill this object from json.
	 * @param j JSON object to fill from
	 * @param cp Cache policy for user records, wether or not we cache users when a message is received
	 * @return A reference to self
	 */
	message& fill_from_json(nlohmann::json* j, cache_policy_t cp = dpp::cp_aggressive);

	/** Build JSON from this object.
	 * @param with_id True if the ID is to be included in the built JSON
	 * @return The JSON text of the message
	 */
	std::string build_json(bool with_id = false, bool is_interaction_response = false) const;

	/**
	 * @brief Returns true if the message was crossposted to other servers
	 * 
	 * @return true if crossposted
	 */
	bool is_crossposted() const;

	/**
	 * @brief Returns true if posted from other servers news channel via webhook
	 * 
	 * @return true if posted from other server
	 */
	bool is_crosspost() const;

	/**
	 * @brief True if embeds have been removed
	 * 
	 * @return true if embeds removed
	 */
	bool supress_embeds() const;

	/**
	 * @brief True if source message was deleted
	 * 
	 * @return true if source message deleted
	 */
	bool is_source_message_deleted() const;

	/**
	 * @brief True if urgent
	 * 
	 * @return true if urgent
	 */
	bool is_urgent() const;

	/**
	 * @brief True if ephemeral (visible only to issuer of a slash command)
	 * 
	 * @return true if ephemeral
	 */
	bool is_ephemeral() const;

	/**
	 * @brief True if loading
	 * 
	 * @return true if loading
	 */
	bool is_loading() const;

	/**
	 * @brief Add a component (button) to message
	 * 
	 * @param c component to add
	 * @return message& reference to self
	 */
	message& add_component(const component& c);

	/**
	 * @brief Add an embed to message
	 * 
	 * @param e embed to add
	 * @return message& reference to self
	 */
	message& add_embed(const embed& e);

	/**
	 * @brief Set the flags
	 * 
	 * @param f flags to set
	 * @return message& reference to self
	 */
	message& set_flags(uint8_t f);

	/**
	 * @brief Set the message type
	 * 
	 * @param t type to set
	 * @return message& reference to self
	 */
	message& set_type(message_type t);

	/**
	 * @brief Set the filename
	 * 
	 * @param fn filename
	 * @return message& reference to self
	 */
	message& set_filename(const std::string &fn);

	/**
	 * @brief Set the file content
	 * 
	 * @param fc raw file content contained in std::string
	 * @return message& reference to self
	 */
	message& set_file_content(const std::string &fc);

	/**
	 * @brief Set the message content
	 * 
	 * @param c message content
	 * @return message& reference to self
	 */
	message& set_content(const std::string &c);
};

/** A group of messages */
typedef std::unordered_map<snowflake, message> message_map;

};
