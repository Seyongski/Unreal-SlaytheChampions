#include "CardStyleDataAsset.h"

UTexture2D* UCardStyleDataAsset::GetBorderImage(ECardRarity Rarity) const
{
    switch (Rarity)
    {
    case ECardRarity::Rare:
        return BorderImage_Rare.LoadSynchronous();
    case ECardRarity::Legendary:
        return BorderImage_Legendary.LoadSynchronous();
    default:
        return BorderImage_Normal.LoadSynchronous();
    }
}

UTexture2D* UCardStyleDataAsset::GetGemImage(ECardRarity Rarity) const
{
    switch (Rarity)
    {
    case ECardRarity::Rare:
        return GemImage_Rare.LoadSynchronous();
    case ECardRarity::Legendary:
        return GemImage_Legendary.LoadSynchronous();
    default:
        return GemImage_Normal.LoadSynchronous();
    }
}