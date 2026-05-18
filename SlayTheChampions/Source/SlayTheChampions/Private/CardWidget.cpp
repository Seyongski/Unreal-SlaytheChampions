#include "CardWidget.h"
#include "CardStyleDataAsset.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/Texture2D.h"

void UCardWidget::SetCardData(const FCardDataRow& InCardData, UCardStyleDataAsset* InStyle)
{
    CurrentCardData = InCardData;

    // Rarity에 맞는 직업별 스타일 이미지 적용
    if (InStyle)
    {
        if (BorderImage)
        {
            UTexture2D* Border = InStyle->GetBorderImage(InCardData.Rarity);
            if (Border) BorderImage->SetBrushFromTexture(Border);
        }

        if (GemImage)
        {
            UTexture2D* Gem = InStyle->GetGemImage(InCardData.Rarity);
            if (Gem) GemImage->SetBrushFromTexture(Gem);
        }
    }

    // 카드별 메인 이미지 적용
    if (MainImage)
    {
        UTexture2D* Main = InCardData.MainImage.LoadSynchronous();
        if (Main) MainImage->SetBrushFromTexture(Main);
    }

    // 텍스트 적용
    if (CardNameText)
        CardNameText->SetText(InCardData.CardName);

    if (DescriptionText)
        DescriptionText->SetText(InCardData.Description);

    if (CostText)
        CostText->SetText(FText::AsNumber(InCardData.Cost));

    OnCardDataSet(InCardData);
}