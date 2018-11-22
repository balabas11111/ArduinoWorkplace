/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Fouad Almalki
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

package io.fouad.jtb.core.beans;

import com.fasterxml.jackson.annotation.JsonProperty;
import com.fasterxml.jackson.annotation.JsonPropertyOrder;

import lombok.Data;
import lombok.EqualsAndHashCode;
import lombok.ToString;

/**
 * This object represents an incoming callback query from
 * a callback button in an inline keyboard. If the button
 * that originated the query was attached to a message sent
 * by the bot, the field message will be presented. If the
 * button was attached to a message sent via the bot (in
 * inline mode), the field inline_message_id will be
 * presented.
 */
@Data
@ToString
@EqualsAndHashCode
public class CallbackQuery
{
	/**
	 * Unique identifier for this query.
	 */
	@JsonProperty("id")
	private String id;
	
	/**
	 * Sender.
	 */
	@JsonProperty("from")
	private User from;
	
	/**
	 * Optional. Message with the callback button that originated
	 * the query. Note that message content and message date will
	 * not be available if the message is too old.
	 */
	@JsonProperty("message")
	private Message message;
	
	/**
	 * Optional. Identifier of the message sent via the bot in
	 * inline mode, that originated the query.
	 */
	@JsonProperty("inline_message_id")
	private String inline_message_id;
	
	/**
	 * Data associated with the callback button. Be aware that
	 * a bad client can send arbitrary data in this field.
	 */
	@JsonProperty("data")
	private String data;
	
	@JsonProperty
	private String chat_instance;
	
	public CallbackQuery(){}
	
	public CallbackQuery(String id, User from, Message message, String inline_message_id, String data)
	{
		this.id = id;
		this.from = from;
		this.message = message;
		this.inline_message_id = inline_message_id;
		this.data = data;
	}
	
}