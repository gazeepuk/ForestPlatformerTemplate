// Developed by Ivan Piankouski. All Rights Reserved


#include "CoinCollectableBase.h"

#include "Interfaces/CoinsWalletInterface.h"


void ACoinCollectableBase::NativeOnCollected(AActor* InInstigator)
{
	Super::NativeOnCollected(InInstigator);
	if(InInstigator && InInstigator->Implements<UCoinsWalletInterface>())
	{
		ICoinsWalletInterface::Execute_AddCoins(InInstigator, CoinsValue);
	}
}
