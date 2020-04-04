package com.balabas.smarthouse.server.entity.model.entityfields;

import java.util.Collections;
import java.util.Date;
import java.util.List;
import java.util.Optional;
import java.util.Set;

import javax.persistence.CascadeType;
import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.persistence.FetchType;
import javax.persistence.Inheritance;
import javax.persistence.InheritanceType;
import javax.persistence.JoinColumn;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;
import javax.persistence.Transient;

import org.thymeleaf.util.StringUtils;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.AbstractItemType;
import com.balabas.smarthouse.server.entity.model.descriptor.EntityFieldClassView;
import com.balabas.smarthouse.server.entity.model.enabledvalue.EntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.exception.BadValueException;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.google.common.collect.Lists;

import lombok.EqualsAndHashCode;
import lombok.Getter;
import lombok.Setter;
import lombok.ToString;

@SuppressWarnings("rawtypes")
@ToString(callSuper = true)
@EqualsAndHashCode(callSuper = true, exclude = {"entity", "enabledValues"})
@Inheritance(strategy = InheritanceType.SINGLE_TABLE)
@javax.persistence.Entity
public abstract class EntityField<T> extends ItemAbstract implements IEntityField<T> {

	@JsonIgnore
	@Getter @Setter
	@ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name="entity_id", nullable=false)
	private Entity entity;

	@Getter	@Setter
	protected boolean readOnly;
	
	@Getter	@Setter
	protected boolean active;
	
	@Getter	@Setter
	protected boolean calculated;
	
	@Getter	@Setter
	protected String descriptionField;

	@Setter
	protected String measure;
	
	@Getter	@Setter
	@Enumerated(EnumType.STRING)
	protected EntityFieldClassView viewClass;

	@Getter	@Setter
	@OneToMany(targetEntity = EntityFieldEnabledValue.class, mappedBy="entityField", fetch = FetchType.EAGER, cascade = CascadeType.ALL)
	protected Set<IEntityFieldEnabledValue> enabledValues;

	@Getter	@Setter
	protected String templateName;

	public abstract Class getClazz();

	public abstract boolean setValue(T value);
	
	protected abstract T fromString(String value);

	@Transient
	@Getter	@Setter
	private Date lastDate;
	
	@Transient
	@Getter @Setter
	private String valueTmp;
	
	@Override
	public void setName(String name) {
		super.setName(name);

		if (name != null && !name.isEmpty() && name.contains(":")) {
			String tmplName = name.substring(name.lastIndexOf(":") + ":".length());
			setTemplateName(tmplName);
		} else {
			setTemplateName(name);
		}
	}

	@Override
	public void setValueWithCheck(T value) throws BadValueException {
		validateValue(value);

		setValue(value);
	}
	
	@Override
	public void validateValue(T value) throws IllegalArgumentException{
		if (value != null && enabledValues != null && !enabledValues.isEmpty()
				&& enabledValues.stream().noneMatch(e -> e.getValue().equals(value))) {
			setValue(null);
			throw new IllegalArgumentException();
		}
	}
	
	@Override
	public void setValueStr(String value) throws BadValueException {
		setValueWithCheck(fromString(value));
	}
	
	@Override
	public Float getValueTmpAsFloat() {
		Float result = 0F;
		
		try {
			result = Float.valueOf(getValueTmp());	
		}catch(Exception e) {}
		
		return result;
	}
	
	@Override
	public Integer getValueTmpAsInteger() {
		Integer result = 0;
		
		try {
			result = Integer.valueOf(getValueTmp());	
		}catch(Exception e) {}
		
		return result;
	}
	
	@Override
	public boolean setValueWithNoCheck(T value) {
		return setValue(value);
	}
	
	@Override
	public boolean setValueWithNoCheckStr(String value) {
		return setValueWithNoCheck(fromString(value));
	}

	@JsonIgnore
	@Override
	public String getValueStr() {
		if (getValue() == null) {
			return null; 
		}
		String result = getValue().toString();
		return result;
	}
	
	@JsonIgnore
	@Override
	public String getMeasure() {
		return StringUtils.isEmpty(measure)?"":measure;
	}

	@JsonIgnore
	@Override
	public Object getValueObj() {
		return getValue();
	}
	
	@Override
	public IEntityFieldEnabledValue getEntityFieldEnabledValue(T value) {
		if (enabledValues == null || enabledValues.isEmpty()) {
			return null;
		}

		return enabledValues.stream().filter(ev -> ev.getValue() != null && ev.getValue().equals(value))
				.findFirst().orElse(null);
	}
	
	@Override
	public IEntityFieldEnabledValue getEntityFieldEnabledValueByValueStr(String value) {
		if (enabledValues == null || enabledValues.isEmpty()) {
			return null;
		}

		return enabledValues.stream().filter(ev -> ev.getValueStr() != null && ev.getValueStr().equals(value))
				.findFirst().orElse(null);
	}

	@JsonIgnore
	@Override
	public IEntityFieldEnabledValue getEntityFieldEnabledValueByCurrentValue() {
		if (enabledValues == null || enabledValues.isEmpty()) {
			return null;
		}

		return enabledValues.stream().filter(ev -> (ev.getValue() != null && ev.getValue().equals(getValue())) || (getValue()==null && Boolean.FALSE.equals(ev.getValue())))
				.findFirst().orElse(null);
	}
	
	@JsonIgnore
	@Override
	public boolean isButton() {
		return Boolean.class.equals(getClazz())
				&& getViewClass().isButton();
	}
	
	@Override
	public String getNameDescriptionByDescriptionField() {
		return getName() +" (" + getDescriptionByDescriptionField() + ")";
	}
	
	@Override
	public String getDescriptionByDescriptionField() {
		if(StringUtils.isEmpty(descriptionField)) {
			return Optional.ofNullable(description).orElse("");
		}
		
		Optional<IEntityField> descrField = Optional.ofNullable(entity.getEntityField(descriptionField));
		
		if(descrField.isPresent()) {
			return Optional.ofNullable(descrField.get().getValueStr()).orElse(description);
		}
		
		return description;
	}
	
	@Override
	public String getEmojiDescriptionByDescriptionFieldWithParent() {
		return getEntity().getEmojiDescriptionByDescriptionField() + " " + getEmojiDescriptionByDescriptionField() ;
	}
	
	@Override
	public String getFullName() {
		return getEntity().getDevice().getName()+" "+getEntity().getName()+" "+getName();
	}
	
	@Override
	public String getItemClassId() {
		return super.getItemClassId() + this.getClazz().getSimpleName();
	}
	
	@Override
	public String getValueMeasureStr() {
		return getValueStr() + " " + getMeasure();
	}
	
	@Override
	public void setParent(IItemAbstract parent) {
		if(parent!=null && !Entity.class.isAssignableFrom(parent.getClass())) {
			throw new IllegalArgumentException("IEntity expected");
		}
		setEntity((Entity) parent);
	}
	
	@Override
	public Set<IEntityFieldEnabledValue> getEntityFieldEnabledValues() {
		if(this.getEnabledValues() == null) {
			return Collections.emptySet();
		}
		return this.getEnabledValues();
	}
	
	@Override
	public String getValueStrByEntityFieldEnabledValueDescription() {
		IEntityFieldEnabledValue enVal = getEntityFieldEnabledValueByCurrentValue();
		String result = null;
		
		if(enVal != null) {
			if (Float.class.isAssignableFrom(enVal.getValue().getClass())) {
				result = enVal.getEmoji() + " " + enVal.getDescription();
			} else {
				result = enVal.getDescription();
			}
		} else {
			result = getValueStr();
		}
		
		return result;
	}
	
	
	@Override
	public AbstractItemType getItemAbstractType() {
		return AbstractItemType.ENTITY_FIELD;
	}
	
	@Override
	public String getParentNamesChain() {
		return getEntity().getParentNamesChain() + "-" + getName()+":" + getDescriptionByDescriptionField();
	}
	
	@Override
	public List<IItemAbstract> getAllChildren() {
		return Lists.newArrayList();
	}
}
