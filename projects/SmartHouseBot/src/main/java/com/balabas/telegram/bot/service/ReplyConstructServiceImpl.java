package com.balabas.telegram.bot.service;

import java.util.List;

import lombok.extern.log4j.Log4j2;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import io.fouad.jtb.core.TelegramBotApi;
import io.fouad.jtb.core.beans.CallbackQuery;
import io.fouad.jtb.core.beans.Message;

import com.balabas.telegram.bot.model.TelegramMessage;
import com.balabas.telegram.bot.replyconstructors.ReplyConstructor;

@Log4j2
@Service
public class ReplyConstructServiceImpl implements ReplyConstructService {

    @Autowired
    List<ReplyConstructor> constructors;

    @Override
    public TelegramMessage constructReplyMessage(Message msgIn,
            TelegramMessage replyMessage) {

        log.info("constructReplyMessage messageText="+msgIn.getText()+" chatId="+msgIn.getChat().getId());

        ReplyConstructor constructor = getReplyConstructor(msgIn);

        if (constructor != null) {
            return constructor.constructReplyMessage(msgIn, replyMessage);
        }

        return null;
    }

    private ReplyConstructor getReplyConstructor(Message msgIn) {
        for (ReplyConstructor constructor : constructors) {
            if (constructor.isTarget(msgIn)) {
                return constructor;
            }
        }
        return null;
    }
    
    private ReplyConstructor getReplyConstructor(CallbackQuery query) {
        for (ReplyConstructor constructor : constructors) {
            if (constructor.isTarget(query)) {
                return constructor;
            }
        }
        return null;
    }

    @Override
    public TelegramMessage constructReplyMessage(TelegramBotApi api,
            Message msgIn, long chatId) {
        long chat = chatId > 0 ? chatId : msgIn.getChat().getId();

        return constructReplyMessage(msgIn, new TelegramMessage(api, null,
                chat));
    }

    @Override
    public TelegramMessage constructReplyMessage(TelegramBotApi api,
            Message msgIn) {
        return constructReplyMessage(api, msgIn, 0);
    }

    @Override
    public TelegramMessage constructReplyMessage(TelegramBotApi api,
            CallbackQuery query) {
        
        log.info("constructReplyMessage queryText="+query.getData()+" chatId="+TelegramComponentsUtil.getChatId(query));
        
        ReplyConstructor constructor = getReplyConstructor(query);
        if (constructor != null) {
            return constructor.constructReplyMessage(query, new TelegramMessage(api, null,
                    query.getMessage().getChat().getId()));
        }

        return null;
    }
}
