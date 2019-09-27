package com.balabas.telegram.bot.service;

import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.TransformerException;

import org.xml.sax.SAXException;

import com.balabas.telegram.bot.model.TelegramMessage;

import io.fouad.jtb.core.TelegramBotApi;
import io.fouad.jtb.core.beans.CallbackQuery;
import io.fouad.jtb.core.beans.Message;

public interface ReplyConstructService {
    
    TelegramMessage constructReplyMessage(Message msgIn, TelegramMessage replyMessage);
    
    TelegramMessage constructReplyMessage(TelegramBotApi arg0,Message msgIn, long chatId);
    
    TelegramMessage constructReplyMessage(TelegramBotApi arg0,Message msgIn);
    
    TelegramMessage constructReplyMessage(TelegramBotApi arg0,CallbackQuery query) throws ParserConfigurationException, SAXException, IOException, TransformerException;

}