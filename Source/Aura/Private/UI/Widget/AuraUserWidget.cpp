// Copyright Grégoire Lebas


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* NewController)
{
	WidgetController = NewController;
	WidgetControllerSet();
}
